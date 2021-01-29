#include "nyast/BaseClassLibrary/False.hpp"

namespace nyast
{

Oop Oop::falseValue()
{
    return False::uniqueInstance();
}

std::string False::asString() const
{
    return "false";
}

} // End of namespace nyast
