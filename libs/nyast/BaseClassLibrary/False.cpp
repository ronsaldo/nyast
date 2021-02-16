#include "nyast/BaseClassLibrary/False.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

Oop Oop::falseValue()
{
    return False::uniqueInstance();
}

static NativeClassRegistration<False> falseClassRegistration;

MethodCategories False::__instanceMethods__()
{
    return MethodCategories{
        {"converting", {
            makeMethodBinding("asString", &SelfType::asString),
        }}
    };
}

std::string False::asString() const
{
    return "false";
}

bool False::asBoolean8() const
{
    return false;
}

} // End of namespace nyast
