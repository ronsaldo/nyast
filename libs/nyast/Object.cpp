#include "nyast/Object.hpp"
#include <sstream>

namespace nyast
{
std::string Oop::asString() const
{
    if(isPointer())
        return asObjectPtr()->asString();

    std::ostringstream out;
    out << std::ends;
    return out.str();
}

std::string Oop::printString() const
{
    if(isPointer())
        return asObjectPtr()->printString();

    std::ostringstream out;
    out << std::ends;
    return out.str();
}

std::string Oop::getClassName() const
{
    return getClass()->asString();
}

} // End of namespace nyast
