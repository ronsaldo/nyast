#include "nyast/RuntimeEnvironment/SimpleGCHeap.hpp"

namespace nyast
{

GCHeap* GCHeap::createDefault()
{
    return new SimpleGCHeap();
}

uint8_t *SimpleGCHeap::allocateObjectMemory(size_t allocationSize)
{
    auto result = new uint8_t[allocationSize];
    {
        std::unique_lock<std::mutex> l(mutex);
        allocatedObjects.insert(result);
    }
    return result;
}

bool SimpleGCHeap::isValidPointerToObject(uint8_t *pointer)
{
    std::unique_lock<std::mutex> l(mutex);
    return allocatedObjects.find(pointer) != allocatedObjects.end();
}

void SimpleGCHeap::deleteAllObjectsThatSatisfy(const GCHeapSweepPredicate &predicate)
{
    std::unique_lock<std::mutex> l(mutex);
    auto it = allocatedObjects.begin();
    while(it != allocatedObjects.end())
    {
        auto pointer = *it;
        if(predicate(pointer))
        {
            delete [] pointer;
            allocatedObjects.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

} // End of namespace nyast
