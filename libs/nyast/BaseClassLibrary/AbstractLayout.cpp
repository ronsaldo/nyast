#include "nyast/BaseClassLibrary/AbstractLayout.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{

static NativeClassRegistration<AbstractLayout> abstractLayoutClassRegistration;

SlotDefinitions AbstractLayout::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("host", &SelfType::host),
    };
}

MethodCategories AbstractLayout::__instanceMethods__()
{
    return MethodCategories{};
}

} // End of namespace nyast
