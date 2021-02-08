#include "nyast/BaseClassLibrary/Association.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{
SlotDefinitions Association::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("key", &SelfType::key),
        makeMemberSlot("value", &SelfType::value),
    };
}

MethodBindings Association::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("isAssociation", &SelfType::isAssociation),

        // Accessing
        makeGetterMethodBinding("key", &SelfType::key),
        makeSetterMethodBinding("key:", &SelfType::key),

        makeGetterMethodBinding("value", &SelfType::value),
        makeSetterMethodBinding("value:", &SelfType::value),
    };
}

bool Association::isAssociation() const
{
    return true;
}

Oop Association::getKey() const
{
    return key;
}

Oop Association::evaluateValue() const
{
    return value;
}

} // End of namespace nyast
