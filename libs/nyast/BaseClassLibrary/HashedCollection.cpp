#include "nyast/BaseClassLibrary/HashedCollection.hpp"
#include "nyast/BaseClassLibrary/Array.hpp"

namespace nyast
{
void HashedCollection::initialize()
{
    tally = 0;
    array = Oop::fromObjectPtr(basicNewInstance<Array> (5));
}

void HashedCollection::fullCheck()
{
    auto capacity = array->getBasicSize();
    if(capacity - tally < std::max(capacity / 4u, size_t(1u)))
        self()->grow();
}

} // End of namespace nyast
