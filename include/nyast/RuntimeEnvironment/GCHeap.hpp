#ifndef NYAST_GC_HEAP_HPP
#define NYAST_GC_HEAP_HPP

#include "nyast/Oop.hpp"
#include "nyast/DllExport.hpp"
#include <functional>

namespace nyast
{

typedef std::function<bool (uint8_t *)> GCHeapSweepPredicate;

/**
 * Interface for a garbage collected heap implementation.
 */
class NYAST_CORE_EXPORT GCHeap
{
public:
    virtual ~GCHeap();
    static GCHeap* createDefault();

    virtual uint8_t *allocateObjectMemory(size_t allocationSize) = 0;
    virtual bool isValidPointerToObject(uint8_t *pointer) = 0;
    virtual void deleteAllObjectsThatSatisfy(const GCHeapSweepPredicate &predicate) = 0;
};

} // End of namespace nyast

#endif /* NYAST_GC_HEAP_HPP */
