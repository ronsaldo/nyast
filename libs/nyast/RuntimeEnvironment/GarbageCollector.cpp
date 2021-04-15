#include "nyast/RuntimeEnvironment/GarbageCollector.hpp"
#include "nyast/RuntimeEnvironment/RuntimeEnvironment.hpp"

namespace nyast
{

class SimpleGarbageCollector : public GarbageCollector
{
public:
    SimpleGarbageCollector();
    ~SimpleGarbageCollector();

    uint8_t* allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void(uint8_t*)>& memoryInitializationFunction);

    void registerGlobalOopRoots(size_t rootCount, Oop* roots);
    void unregisterGlobalOopRoots(size_t rootCount, Oop* roots);
    void registerThisThread();
    void unregisterThisThread(StackRangeRecord* record);
    void safePoint();

private:
    std::mutex collectorMutex;
    std::unordered_map<Oop*, size_t> globalOopRoots;
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

uint8_t *SimpleGarbageCollector::allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void (uint8_t*)> &memoryInitializationFunction)
{
    assert(getCurrentStackRangeRecord() != nullptr && "GC collected memory must be allocated in a GC collected stack range.");
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
    printf("TODO: registerThisThread\n");;
}

void SimpleGarbageCollector::unregisterThisThread(StackRangeRecord *record)
{
    printf("TODO: unregisterThisThread record: %p\n", record);
}

void SimpleGarbageCollector::safePoint()
{
    printf("TODO: safePoint\n");;
}

} // End of namespace nyast
