#include "nyast/BaseClassLibrary/Metaclass.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{
static NativeClassRegistration<Metaclass> metaclassClassRegistration;

MethodCategories Metaclass::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeGetterMethodBinding("soleInstance", &SelfType::thisClass),
        }},

        {"printing", {
            makeMethodBinding("asString", &SelfType::asString),
        }},
    };
}

std::string Metaclass::asString() const
{
    if(thisClass.isNotNil())
        return thisClass->asString() + " class";
    return Super::asString();
}

} // End of namespace nyast
