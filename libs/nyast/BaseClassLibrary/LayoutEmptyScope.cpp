#include "nyast/BaseClassLibrary/LayoutEmptyScope.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{

static NativeClassRegistration<LayoutEmptyScope> layoutEmptyScopeClassRegistration;

MethodCategories LayoutEmptyScope::__instanceMethods__()
{
    return MethodCategories{};
}

void LayoutEmptyScope::storeReferenceTypesInGCLayout(Oop gcLayout)
{
    (void)gcLayout;
    // Nothing is required here.
}

} // End of namespace nyast
