#include "nyast/BaseClassLibrary/LayoutAbstractScope.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

static NativeClassRegistration<LayoutAbstractScope> layoutAbstractScopeClassRegistration;

MethodCategories LayoutAbstractScope::__instanceMethods__()
{
    return MethodCategories{};
}

} // End of namespace nyast
