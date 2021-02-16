#include "nyast/BaseClassLibrary/SequenceableCollection.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<SequenceableCollection> sequenceableCollectionClassRegistration;

MethodCategories SequenceableCollection::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding("first", &SelfType::first),
            makeMethodBinding("second", &SelfType::second),
            makeMethodBinding("third", &SelfType::third),
            makeMethodBinding("fourth", &SelfType::fourth),
            makeMethodBinding("fifth", &SelfType::fifth),
            makeMethodBinding("sixth", &SelfType::sixth),
            makeMethodBinding("seventh", &SelfType::seventh),
            makeMethodBinding("eighth", &SelfType::eighth),
            makeMethodBinding("ninth", &SelfType::ninth),
        }}
    };
}

Oop SequenceableCollection::first() const
{
    return self()->at(Oop::fromSize(1));
}

Oop SequenceableCollection::second() const
{
    return self()->at(Oop::fromSize(2));
}

Oop SequenceableCollection::third() const
{
    return self()->at(Oop::fromSize(3));
}

Oop SequenceableCollection::fourth() const
{
    return self()->at(Oop::fromSize(4));
}

Oop SequenceableCollection::fifth() const
{
    return self()->at(Oop::fromSize(5));
}

Oop SequenceableCollection::sixth() const
{
    return self()->at(Oop::fromSize(6));
}

Oop SequenceableCollection::seventh() const
{
    return self()->at(Oop::fromSize(7));
}

Oop SequenceableCollection::eighth() const
{
    return self()->at(Oop::fromSize(8));
}

Oop SequenceableCollection::ninth() const
{
    return self()->at(Oop::fromSize(9));
}



} // End of namespace nyast
