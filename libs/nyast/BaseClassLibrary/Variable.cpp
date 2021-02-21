#include "nyast/BaseClassLibrary/Variable.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{
static NativeClassRegistration<Variable> variableClassRegistration;

SlotDefinitions Variable::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("name", &SelfType::name),
    };
}

MethodCategories Variable::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeGetterMethodBinding("name", &SelfType::name),
            makeSetterMethodBinding("name:", &SelfType::name),
        }},
    };
}

Oop Variable::getName() const
{
    return name;
}

} // End of namespace nyast
