#include "nyast/BaseClassLibrary/HashedCollection.hpp"
#include "nyast/BaseClassLibrary/Array.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{

static NativeClassRegistration<HashedCollection> hashedCollectionClassRegistration;

MethodCategories HashedCollection::__instanceMethods__()
{
    return MethodCategories{
        {"initialization", {
            makeMethodBinding("initialize", &SelfType::initialize),
        }},

        {"private", {
            makeMethodBinding("grow", &SelfType::grow),
            makeMethodBinding("fullCheck", &SelfType::fullCheck),
        }}
    };
}

SlotDefinitions HashedCollection::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("array", &SelfType::array),
    };
}

Oop HashedCollection::initialize()
{
    tally = 0;
    array = staticOopBasicNewInstance<Array> (5);
    return self();
}

void HashedCollection::fullCheck()
{
    auto capacity = array->getBasicSize();
    if(capacity - tally < std::max(capacity / 4u, size_t(1u)))
        self()->grow();
}

void HashedCollection::grow()
{
    auto oldArray = array;
    auto oldCapacity = oldArray->getBasicSize();
    auto newCapacity = oldCapacity*2;

    tally = 0;
    array = Oop::fromObjectPtr(staticBasicNewInstance<Array> (newCapacity));

    for(size_t i = 1; i <= oldCapacity; ++i)
    {
        auto element = oldArray->basicAt(i);
        if(element.isNotNil())
        {
            add(element);
        }
    }
}

} // End of namespace nyast
