#include "nyast/Object.hpp"
#include <sstream>

namespace nyast
{
std::string Oop::asString() const
{
    if(isPointer())
        return asObjectPtr()->asString();

    std::ostringstream out;
    if(isSmallInteger())
        out << decodeSmallInteger();
    else if(isSmallFloat())
        out << decodeSmallFloat();
    else if(isCharacter())
        out << decodeCharacter();
    out << std::ends;
    return out.str();
}

std::string Oop::printString() const
{
    if(isPointer())
        return asObjectPtr()->printString();

    std::ostringstream out;
    if(isSmallInteger())
        out << decodeSmallInteger();
    else if(isSmallFloat())
        out << decodeSmallFloat();
    else if(isCharacter())
        out << decodeCharacter();
    out << std::ends;
    return out.str();
}

std::string Oop::getClassName() const
{
    return getClass()->asString();
}

} // End of namespace nyast
