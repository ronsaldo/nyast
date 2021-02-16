#include "nyast/BaseClassLibrary/Dictionary.hpp"
#include "nyast/BaseClassLibrary/Array.hpp"
#include "nyast/BaseClassLibrary/Association.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

static NativeClassRegistration<Dictionary> dictionaryClassRegistration;

MethodCategories Dictionary::__instanceMethods__()
{
    return MethodCategories{
        {"adding", {
            makeMethodBinding("add:", &SelfType::add),
        }},

        {"accessing", {
            makeMethodBinding("at:put:", &SelfType::atPut),
            makeMethodBinding("atOrNil:", &SelfType::atOrNil),
        }},

        {"private", {
            makeMethodBinding("scanFor:", &SelfType::scanFor),
        }},
    };
}

Oop Dictionary::scanFor(Oop key) const
{
    auto hash = key->hash();
    auto finish = array->getBasicSize();
    auto start = (hash % finish) + 1;

    for(size_t i = start; i <= finish; ++i)
    {
        auto element = array->basicAt(i);
        if(element.isNil() || element->getKey()->equals(key))
            return Oop::fromSize(i);
    }

    for(size_t i = 1; i < start; ++i)
    {
        auto element = array->basicAt(i);
        if(element.isNil() || element->getKey()->equals(key))
            return Oop::fromSize(i);
    }

    return Oop::fromSize(0);
}

Oop Dictionary::atOrNil(Oop key) const
{
    auto elementIndexOop = self()->scanFor(key);
    if(!elementIndexOop.isSmallInteger())
        return Oop::nil();

    auto elementIndex = elementIndexOop.decodeSmallInteger();
    if(elementIndex == 0)
        return Oop::nil();

    return array->basicAt(elementIndex)
        ->getValue();
}

Oop Dictionary::add(Oop association)
{
    auto elementIndex = self()->scanFor(association->getKey()).decodeSmallInteger();
    assert(elementIndex != 0);

    auto existentAssociation = array->basicAt(elementIndex);
    if(existentAssociation.isNil())
    {
        array->basicAtPut(elementIndex, association);
        ++tally;
        fullCheck();
    }
    else
    {
        existentAssociation->setValue(association->getValue());
    }

    return association;
}

Oop Dictionary::atPut(Oop key, Oop value)
{
    auto elementIndex = self()->scanFor(key).decodeSmallInteger();
    assert(elementIndex != 0);

    auto association = array->basicAt(elementIndex);
    if(association.isNil())
    {
        association = Oop::fromObjectPtr(staticBasicNewInstance<Association> (0, key, value));
        array->basicAtPut(elementIndex, association);
        ++tally;
        fullCheck();
    }
    else
    {
        association->setValue(value);
    }

    return value;
}

} // End of namespace nyast
