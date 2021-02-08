#include "nyast/BaseClassLibrary/Metaclass.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{
MethodBindings Metaclass::__instanceMethods__()
{
    return MethodBindings{
        // Accessing
        makeGetterMethodBinding("soleInstance", &SelfType::thisClass),

        // Printing.
        makeMethodBinding("asString", &SelfType::asString),
    };
}

std::string Metaclass::asString() const
{
    if(thisClass.isNotNil())
        return thisClass->asString() + " class";
    return Super::asString();
}

} // End of namespace nyast
