#include "nyast/BaseClassLibrary/Metaclass.hpp"

namespace nyast
{

std::string Metaclass::asString() const
{
    if(thisClass.isNotNil())
        return thisClass->asString() + " class";
    return Super::asString();
}

} // End of namespace nyast
