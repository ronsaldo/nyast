#include "nyast/RuntimeEnvironment/GarbageCollector.hpp"
#include "nyast/RuntimeEnvironment/RuntimeEnvironment.hpp"

#include <atomic>
#include <thread>

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

    void stopTheWorldAndGarbageCollect();
    void performGarbageCollectionCycle();

    std::mutex collectorMutex;
    std::unordered_map<Oop*, size_t> globalOopRoots;

    std::unordered_map<std::thread::id, RegisteredThread> registeredThread;
    size_t activeThreadCount = 0;

    bool isRuntimeInitialized = false;
    std::atomic_bool isStoppingTheWorld;
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
}

SimpleGarbageCollector::~SimpleGarbageCollector()
{
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
    isStoppingTheWorld.store(true, std::memory_order_relaxed);
    safePoint();
}

void SimpleGarbageCollector::performGarbageCollectionCycle()
{
    printf("TODO: Garbage collect\n");
}

uint8_t *SimpleGarbageCollector::allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void (uint8_t*)> &memoryInitializationFunction)
{
    assert(getCurrentStackRangeRecord() != nullptr && "GC collected memory must be allocated in a GC collected stack range.");
    checkCollectionConditionFor(allocationSize);
    auto allocation = new uint8_t[allocationSize];
    memoryInitializationFunction(allocation);
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
        printf("TODO: Garbage collect on last thread leaving\n");
        performGarbageCollectionCycle();
    }
}

void SimpleGarbageCollector::safePoint()
{
    if(!isStoppingTheWorld.load(std::memory_order_relaxed))
        return;

/*    std::unique_lock<std::mutex> l(collectorMutex);
    auto it = registeredThread.find(std::this_thread::get_id());
    assert(it != registeredThread.end());
    assert(it->second.isActive);
*/
    printf("TODO: safePoint stop the world\n");
}

void SimpleGarbageCollector::runtimeHasFinishedInitialization()
{
    isRuntimeInitialized = true;
    std::unique_lock<std::mutex> l(collectorMutex);
    printf("TODO: Peform first garbage collection cycle\n");

}

} // End of namespace nyast
