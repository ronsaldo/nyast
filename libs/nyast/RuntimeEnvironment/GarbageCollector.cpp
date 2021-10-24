#include "nyast/RuntimeEnvironment/GarbageCollector.hpp"
#include "nyast/RuntimeEnvironment/GCHeap.hpp"
#include "nyast/RuntimeEnvironment/RuntimeEnvironment.hpp"
#include "nyast/BaseClassLibrary/GCLayout.hpp"

#include <atomic>
#include <thread>
#include <condition_variable>

namespace nyast
{

class SimpleGarbageCollector : public GarbageCollector
{
public:
    SimpleGarbageCollector();
    ~SimpleGarbageCollector();

    void checkCollectionConditionFor(size_t requiredAllocationSize);
    uint8_t* allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void(uint8_t*)>& memoryInitializationFunction);

    void registerGlobalOopRoots(size_t rootCount, Oop* roots) override;
    void unregisterGlobalOopRoots(size_t rootCount, Oop* roots) override;
    void registerThisThread() override;
    void unregisterThisThread(StackRangeRecord* record) override;
    void safePoint() override;
    
    void runtimeHasFinishedInitialization() override;

private:
    struct RegisteredThread
    {
        bool isActive = false;
        StackRangeRecord *activeStackRangeRecord = nullptr;
    };

    void stopTheWorldBarrier();
    void stopTheWorldAndGarbageCollect();
    void performGarbageCollectionCycle();

    void conservativeMarkStackRange(const StackRangeRecord *stackRange);
    void conservativeMarkStacks();

    void markPhase();
    void markPointer(Oop pointer);
    void expandMarkedObject(Oop markedOop);
    void clearWeakReferences();
    void clearWeakReferencesIn(Oop oop);
    bool clearCollectedWeakReference(Oop &weakReference);
    void sweepPhase();
    void swapColors();
    void collectorThreadLoop();

    std::mutex collectorMutex;
    std::unordered_map<Oop*, size_t> globalOopRoots;

    std::unordered_map<std::thread::id, RegisteredThread> registeredThread;
    size_t activeThreadCount = 0;

    bool isRuntimeInitialized = false;
    std::atomic_bool isStoppingTheWorld = false;

    bool shuttingDown;
    std::thread collectorThread;
    std::condition_variable collectorThreadPendingJobCondition;
    std::condition_variable collectionCycleEnded;

    uint8_t whiteColor;
    uint8_t grayColor;
    uint8_t blackColor;
    std::vector<Oop> markingStack;
    std::vector<Oop> markedObjectWithWeakReferences;
    std::unique_ptr<GCHeap> heap;
};

GarbageCollector::~GarbageCollector()
{
}

GarbageCollector* GarbageCollector::createDefault()
{
    return new SimpleGarbageCollector();
}

SimpleGarbageCollector::SimpleGarbageCollector()
{
    whiteColor = 0;
    grayColor = 1;
    blackColor = 2;

    heap.reset(GCHeap::createDefault());

    {
        std::thread t([=](){
            collectorThreadLoop();
        });
        collectorThread.swap(t);
    }
}

SimpleGarbageCollector::~SimpleGarbageCollector()
{
    {
        std::unique_lock<std::mutex> l(collectorMutex);
        shuttingDown = true;
        isStoppingTheWorld.store(true, std::memory_order_release);
        collectorThreadPendingJobCondition.notify_all();
    }

    collectorThread.join();
}

void SimpleGarbageCollector::checkCollectionConditionFor(size_t requiredAllocationSize)
{
    (void)requiredAllocationSize;
    if(!isRuntimeInitialized)
        return;

    stopTheWorldAndGarbageCollect();
}

void SimpleGarbageCollector::stopTheWorldAndGarbageCollect()
{
    isStoppingTheWorld.store(true, std::memory_order_release);
    stopTheWorldBarrier();
}

void SimpleGarbageCollector::collectorThreadLoop()
{
    for(;;)
    {
        std::unique_lock<std::mutex> l(collectorMutex);
        bool shouldPerformCollectionCycle;
        while(!(
            (shouldPerformCollectionCycle = isStoppingTheWorld.load(std::memory_order_acquire) && activeThreadCount == 0) ||
            shuttingDown))
            collectorThreadPendingJobCondition.wait(l);

        if(shouldPerformCollectionCycle)
            performGarbageCollectionCycle();
        
        if(shuttingDown)
            return;
    }
}

void SimpleGarbageCollector::markPointer(Oop pointer)
{
    if(pointer.isImmediate() || pointer.isNull())
        return;
    
    assert(heap->isValidPointerToObject(reinterpret_cast<uint8_t*> (pointer.value)));
    auto objectPointer = pointer.asObjectPtr();
    if(objectPointer->__gcMarkingColor != whiteColor)
        return;

    objectPointer->__gcMarkingColor = grayColor;
    markingStack.push_back(pointer);
}

void SimpleGarbageCollector::conservativeMarkStackRange(const StackRangeRecord *stackRange)
{
    auto startAddress = reinterpret_cast<uintptr_t> (stackRange->startAddress);
    auto endAddress = reinterpret_cast<uintptr_t> (stackRange->endAddress);
    assert(endAddress <= startAddress);

    // Scan the pointers in the stack range
    for(uintptr_t currentStackAddress = endAddress; currentStackAddress < startAddress; currentStackAddress += sizeof(uintptr_t))
    {
        uintptr_t stackValue = *reinterpret_cast<uintptr_t*> (currentStackAddress);
        if(stackValue == 0)
            continue;

        auto potentialOop = Oop(stackValue);
        if(!potentialOop.isPointer())
            continue;

        if(!heap->isValidPointerToObject(reinterpret_cast<uint8_t*> (stackValue)))
            continue;

        markPointer(potentialOop);
    }
}

void SimpleGarbageCollector::conservativeMarkStacks()
{
    for(const auto &[threadId, threadInfo] : registeredThread)
    {
        for(auto stackRange = threadInfo.activeStackRangeRecord; stackRange; stackRange = stackRange->previous)
            conservativeMarkStackRange(stackRange);
    }
}

void SimpleGarbageCollector::expandMarkedObject(Oop markedOop)
{
    auto markedObject = markedOop.asObjectPtr();
    assert(markedObject->__gcMarkingColor == grayColor);

    // Fetch the object class, and mark it.
    auto markedObjectClass = markedOop->getClass();
    assert(markedObjectClass.isNotNilOrNull());
    markPointer(markedObjectClass);

    // Fetch the object gc layout, and mark it.
    auto gcLayoutOop = markedObjectClass->getGCLayout();
    assert(gcLayoutOop.isNotNilOrNull());
    markPointer(markedObjectClass);
    auto gcLayout = gcLayoutOop.as<GCLayout> ();
    auto variableDataSize = markedObject->__variableDataSize;

    // Does this object contain weak references?
    if(gcLayout->hasFixedReferencesOfType(GCReferenceType::Weak) ||
        (variableDataSize > 0 && gcLayout->variableDataLayout == GCReferenceType::Weak))
        markedObjectWithWeakReferences.push_back(markedOop);

    // Does this object contain strong references in the fixed layout?
    if(gcLayout->hasFixedReferencesOfType(GCReferenceType::Strong))
    {
        auto oopWords = reinterpret_cast<Oop*> (markedOop.value);
        gcLayout->fixedLayoutDo([&](size_t wordIndex, GCReferenceType referenceType) {
            if(referenceType == GCReferenceType::Strong)
                markPointer(oopWords[wordIndex]);
        });
    }

    // Does this object contain strong references in the variable layout?
    if(gcLayout->variableDataLayout == GCReferenceType::Strong && variableDataSize > 0)
    {
        auto variableWords = reinterpret_cast<Oop*> (markedOop.value + gcLayout->instanceSize);
        for(size_t i = 0; i < variableDataSize; ++i)
            markPointer(variableWords[i]);
    }

    markedObject->__gcMarkingColor = blackColor;
}

void SimpleGarbageCollector::markPhase()
{
    // Mark all of the roots.
    conservativeMarkStacks();
    for(auto [rootArray, rootArraySize] : globalOopRoots)
    {
        for(size_t i = 0; i < rootArraySize; ++i)
            markPointer(rootArray[i]);
    }

    // Expand from the marked roots.
    while(!markingStack.empty())
    {
        auto oop = markingStack.back();
        markingStack.pop_back();
        expandMarkedObject(oop);
    }
}

void SimpleGarbageCollector::clearWeakReferences()
{
    for(auto &oop : markedObjectWithWeakReferences)
        clearWeakReferencesIn(oop);
    markedObjectWithWeakReferences.clear();
}

void SimpleGarbageCollector::clearWeakReferencesIn(Oop oop)
{
    auto object = oop.asObjectPtr();
    assert(object->__gcMarkingColor == blackColor);

    // Fetch the object class.
    auto objectObjectClass = oop->getClass();
    assert(objectObjectClass.isNotNilOrNull());

    // Fetch the object gc layout
    auto gcLayoutOop = objectObjectClass->getGCLayout();
    assert(gcLayoutOop.isNotNilOrNull());
    auto gcLayout = gcLayoutOop.as<GCLayout> ();
    auto variableDataSize = object->__variableDataSize;

    // Does this object contain weak references in the fixed layout?
    if(gcLayout->hasFixedReferencesOfType(GCReferenceType::Weak))
    {
        auto oopWords = reinterpret_cast<Oop*> (oop.value);
        gcLayout->fixedLayoutDo([&](size_t wordIndex, GCReferenceType referenceType) {
            if(referenceType == GCReferenceType::Weak)
                clearCollectedWeakReference(oopWords[wordIndex]);
        });
    }

    // Does this object contain strong references in the variable layout?
    if(gcLayout->variableDataLayout == GCReferenceType::Weak && variableDataSize > 0)
    {
        auto variableWords = reinterpret_cast<Oop*> (oop.value + gcLayout->instanceSize);
        for(size_t i = 0; i < variableDataSize; ++i)
            clearCollectedWeakReference(variableWords[i]);
    }
}

bool SimpleGarbageCollector::clearCollectedWeakReference(Oop &weakReference)
{
    // Make sure this is a valid pointer.
    if(weakReference.isImmediate() || weakReference.isNil())
        return false;

    assert(!weakReference.isNull());

    auto referencedColor = weakReference.asObjectPtr()->__gcMarkingColor;
    assert(referencedColor != grayColor);

    // Is this a collected reference?
    if(referencedColor == whiteColor)
    {
        weakReference = Oop::nil();
        return true;
    }

    return false;
}


void SimpleGarbageCollector::sweepPhase()
{
    auto deletedColor = whiteColor;
    heap->deleteAllObjectsThatSatisfy([=](uint8_t *objectAddress) {
        auto objectColor = reinterpret_cast<NyastObject*> (objectAddress)->__gcMarkingColor;
        return objectColor == deletedColor;
    });
}

void SimpleGarbageCollector::swapColors()
{
    std::swap(whiteColor, blackColor);
}

void SimpleGarbageCollector::performGarbageCollectionCycle()
{
    markPhase();
    sweepPhase();
    swapColors();

    isStoppingTheWorld.store(false, std::memory_order_release);
    collectionCycleEnded.notify_all();
}

uint8_t *SimpleGarbageCollector::allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void (uint8_t*)> &memoryInitializationFunction)
{
    assert(getCurrentStackRangeRecord() != nullptr && "GC collected memory must be allocated in a GC collected stack range.");
    checkCollectionConditionFor(allocationSize);
    auto allocation = heap->allocateObjectMemory(allocationSize);
    memoryInitializationFunction(allocation);
    reinterpret_cast<NyastObject*> (allocation)->__gcMarkingColor = whiteColor;
    return allocation;
}

void SimpleGarbageCollector::registerGlobalOopRoots(size_t rootCount, Oop *roots)
{
    std::unique_lock<std::mutex> l(collectorMutex);
    globalOopRoots[roots] = rootCount;
}

void SimpleGarbageCollector::unregisterGlobalOopRoots(size_t rootCount, Oop *roots)
{
    (void)rootCount;
    std::unique_lock<std::mutex> l(collectorMutex);
    auto it = globalOopRoots.find(roots);
    if(it != globalOopRoots.end())
        globalOopRoots.erase(it);
}

void SimpleGarbageCollector::registerThisThread()
{
    std::unique_lock<std::mutex> l(collectorMutex);
    while(isStoppingTheWorld.load(std::memory_order_acquire))
        collectionCycleEnded.wait(l);

    auto selfId = std::this_thread::get_id();
    auto it = registeredThread.find(selfId);
    if(it != registeredThread.end())
    {
        assert(!it->second.isActive);
        it->second.isActive = true;
        it->second.activeStackRangeRecord = nullptr;
    }
    else
    {
        registeredThread.insert(std::make_pair(selfId, RegisteredThread{true, nullptr}));
    }
    ++activeThreadCount;
}

void SimpleGarbageCollector::unregisterThisThread(StackRangeRecord *record)
{
    std::unique_lock<std::mutex> l(collectorMutex);
    auto it = registeredThread.find(std::this_thread::get_id());
    assert(it != registeredThread.end());
    auto &threadRecord = it->second;
    threadRecord.activeStackRangeRecord = record;

    assert(threadRecord.isActive);
    assert(activeThreadCount > 0);
    threadRecord.isActive = false;
    --activeThreadCount;
    if(activeThreadCount == 0)
    {
        collectorThreadPendingJobCondition.notify_all();
    }
}

void SimpleGarbageCollector::safePoint()
{
    if(!isStoppingTheWorld.load(std::memory_order_acquire))
        return;

    stopTheWorldBarrier();
}

void SimpleGarbageCollector::runtimeHasFinishedInitialization()
{
    isRuntimeInitialized = true;
    stopTheWorldBarrier();
}

void SimpleGarbageCollector::stopTheWorldBarrier()
{
    // Release the current thread from the GC here.
    withoutGarbageCollectedStack([]() {
        // Nothing is required here.
    });
}
} // End of namespace nyast
