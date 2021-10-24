#ifndef NYAST_GC_SIMPLE_GC_HEAP_HPP
#define NYAST_GC_SIMPLE_GC_HEAP_HPP

#include "GCHeap.hpp"
#include <unordered_set>
#include <mutex>

namespace nyast
{

/**
 * Interface for a garbage collected heap implementation.
 */
class NYAST_CORE_EXPORT SimpleGCHeap : public GCHeap
{
public:
    virtual uint8_t *allocateObjectMemory(size_t allocationSize) override;
    virtual bool isValidPointerToObject(uint8_t *pointer) override;
    virtual void deleteAllObjectsThatSatisfy(const GCHeapSweepPredicate &predicate) override;

private:
    std::mutex mutex;
    std::unordered_set<uint8_t*> allocatedObjects;
};

} // End of namespace nyast

#endif /* NYAST_GC_SIMPLE_GC_HEAP_HPP */
