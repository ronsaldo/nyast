#include "nyast/BaseClassLibrary/True.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

Oop Oop::trueValue()
{
    return True::uniqueInstance();
}

MethodCategories True::__instanceMethods__()
{
    return MethodCategories{
        {"printing", {
            makeMethodBinding("asString", &SelfType::asString),            
        }},
    };
}

static NativeClassRegistration<True> trueClassRegistration;

std::string True::asString() const
{
    return "true";
}

bool True::asBoolean8() const
{
    return true;
}

} // End of namespace nyast
