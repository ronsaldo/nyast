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
class NYAST_CORE_EXPORT GarbageCollector
{
public:
    virtual ~GarbageCollector();
    static GarbageCollector* createDefault();

    virtual uint8_t *allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void (uint8_t*)> &memoryInitializationFunction) = 0;

    virtual void registerGlobalOopRoots(size_t rootCount, Oop *roots) = 0;
    virtual void unregisterGlobalOopRoots(size_t rootCount, Oop *roots) = 0;
    virtual void registerThisThread() = 0;
    virtual void unregisterThisThread(StackRangeRecord *record) = 0;
    virtual void safePoint() = 0;

    virtual void runtimeHasFinishedInitialization() = 0;
};

} // End of namespace nyast

#endif /* NYAST_GARBAGE_COLLECTOR_HPP */
