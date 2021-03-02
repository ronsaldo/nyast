#include "nyast/Oop.hpp"
#include <thread>

namespace nyast
{

NYAST_CORE_EXPORT uint8_t *allocateAndInitializeObjectMemoryWith(size_t allocationSize, std::function<void (uint8_t*)> memoryInitializationFunction)
{
    assert(getCurrentStackRangeRecord() != nullptr && "GC collected memory must be allocated in a GC collected stack range.");
    auto allocation = new uint8_t[allocationSize];
    memoryInitializationFunction(allocation);
    return allocation;
}

} // End of namespace nyast
