#ifndef NYAST_GARBAGE_COLLECTOR_HPP
#define NYAST_GARBAGE_COLLECTOR_HPP

#include "nyast/Oop.hpp"
#include "nyast/DllExport.hpp"
#include <unordered_map>
#include <mutex>

namespace nyast
{

/**
 * Garbage collector
 */
NYAST_CORE_EXPORT class GarbageCollector
{
public:
    GarbageCollector();
    ~GarbageCollector();

    static GarbageCollector& getCurrent();

    uint8_t *allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void (uint8_t*)> &memoryInitializationFunction);

    void registerGlobalOopRoots(size_t rootCount, Oop *roots);
    void unregisterGlobalOopRoots(size_t rootCount, Oop *roots);
    void registerThisThread();
    void unregisterThisThread(StackRangeRecord *record);
    void safePoint();

private:
    std::mutex collectorMutex;
    std::unordered_map<Oop*, size_t> globalOopRoots;
};

} // End of namespace nyast

#endif /* NYAST_GARBAGE_COLLECTOR_HPP */
