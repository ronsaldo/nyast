#include "nyast/BaseClassLibrary/Variable.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{
SlotDefinitions Variable::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("name", &SelfType::name),
    };
}

MethodBindings Variable::__instanceMethods__()
{
    return MethodBindings{
        // Accessing
        makeGetterMethodBinding("name", &SelfType::name),
        makeSetterMethodBinding("name:", &SelfType::name),
    };
}

} // End of namespace nyast
