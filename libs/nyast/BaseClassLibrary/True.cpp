#include "nyast/BaseClassLibrary/True.hpp"

namespace nyast
{

Oop Oop::trueValue()
{
    return True::uniqueInstance();
}

std::string True::asString() const
{
    return "true";
}

} // End of namespace nyast
