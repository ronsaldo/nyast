#include "nyast/BaseClassLibrary/MethodDictionary.hpp"
#include "nyast/BaseClassLibrary/Array.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{
static NativeClassRegistration<MethodDictionary> methodDictionaryClassRegistration;

SlotDefinitions MethodDictionary::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("methods", &SelfType::methods),
    };
}

MethodCategories MethodDictionary::__instanceMethods__()
{
    return MethodCategories{
        {"adding", {
            makeMethodBinding("add:", &SelfType::add),
        }},

        {"initialization", {
            makeMethodBinding("initialize", &SelfType::initialize),
        }},

        {"accessing", {
            makeMethodBinding("at:put:", &SelfType::atPut),
            makeMethodBinding("atOrNil:", &SelfType::atOrNil),
        }},

        {"private", {
            makeMethodBinding("scanFor:", &SelfType::scanFor),
            makeMethodBinding("grow", &SelfType::grow),
        }},
    };
}

Oop MethodDictionary::initialize()
{
    Super::initialize();
    methods = staticOopBasicNewInstance<Array> (array->getBasicSize());
    return self();
}

Oop MethodDictionary::scanFor(Oop key) const
{
    auto hash = key->identityHash();
    auto finish = array->getBasicSize();
    auto start = (hash % finish) + 1;

    for(size_t i = start; i <= finish; ++i)
    {
        auto element = array->basicAt(i);
        if(element.isNil() || element->identityEquals(key))
            return Oop::fromSize(i);
    }

    for(size_t i = 1; i < start; ++i)
    {
        auto element = array->basicAt(i);
        if(element.isNil() || element->identityEquals(key))
            return Oop::fromSize(i);
    }

    return Oop::fromSize(0);
}

Oop MethodDictionary::add(Oop anAssociation)
{
    return atPut(anAssociation->getKey(), anAssociation->getValue());
}

Oop MethodDictionary::atOrNil(Oop key) const
{
    auto elementIndex = self()->scanFor(key).decodeSmallInteger();
    if(elementIndex == 0)
        return Oop::nil();

    return methods->basicAt(elementIndex);
}

Oop MethodDictionary::atPut(Oop key, Oop value)
{
    auto elementIndex = self()->scanFor(key).decodeSmallInteger();
    assert(elementIndex != 0);

    if(array->basicAt(elementIndex).isNil())
    {
        array->basicAtPut(elementIndex, key);
        methods->basicAtPut(elementIndex, value);
        ++tally;
        fullCheck();
    }
    else
    {
        methods->basicAtPut(elementIndex, value);
    }

    return value;
}

void MethodDictionary::grow()
{
    auto oldArray = array;
    auto oldMethods = methods;
    auto oldCapacity = oldArray->getBasicSize();
    auto newCapacity = oldCapacity*2;

    tally = 0;
    array = Oop::fromObjectPtr(staticBasicNewInstance<Array> (newCapacity));
    methods = Oop::fromObjectPtr(staticBasicNewInstance<Array> (newCapacity));

    for(size_t i = 1; i <= oldCapacity; ++i)
    {
        auto selector = oldArray->basicAt(i);
        if(selector.isNotNil())
        {
            atPut(selector, oldMethods->basicAt(i));
        }
    }
}


} // End of namespace nyast
