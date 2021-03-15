#include "nyast/RuntimeEnvironment/GarbageCollector.hpp"
#include "nyast/RuntimeEnvironment/RuntimeEnvironment.hpp"

namespace nyast
{

GarbageCollector::GarbageCollector()
{
}

GarbageCollector::~GarbageCollector()
{
}

GarbageCollector& GarbageCollector::getCurrent()
{
    return RuntimeEnvironment::getCurrent().getGarbageCollector();
}

uint8_t *GarbageCollector::allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void (uint8_t*)> &memoryInitializationFunction)
{
    assert(getCurrentStackRangeRecord() != nullptr && "GC collected memory must be allocated in a GC collected stack range.");
    auto allocation = new uint8_t[allocationSize];
    memoryInitializationFunction(allocation);
    return allocation;
}

void GarbageCollector::registerGlobalOopRoots(size_t rootCount, Oop *roots)
{
    std::unique_lock<std::mutex> l(collectorMutex);
    globalOopRoots[roots] = rootCount;
}

void GarbageCollector::unregisterGlobalOopRoots(size_t rootCount, Oop *roots)
{
    (void)rootCount;
    std::unique_lock<std::mutex> l(collectorMutex);
    auto it = globalOopRoots.find(roots);
    if(it != globalOopRoots.end())
        globalOopRoots.erase(it);
}

void GarbageCollector::registerThisThread()
{
    printf("TODO: registerThisThread\n");;
}

void GarbageCollector::unregisterThisThread(StackRangeRecord *record)
{
    printf("TODO: unregisterThisThread record: %p\n", record);
}

void GarbageCollector::safePoint()
{
    printf("TODO: safePoint\n");;
}

NYAST_CORE_EXPORT uint8_t *allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void (uint8_t*)> &memoryInitializationFunction)
{
    return GarbageCollector::getCurrent().allocateAndInitializeObjectMemoryWith(allocationSize, memoryInitializationFunction);
}

} // End of namespace nyast
