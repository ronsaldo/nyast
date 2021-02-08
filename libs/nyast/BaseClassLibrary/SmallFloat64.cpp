#include "nyast/BaseClassLibrary/SmallFloat64.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

MethodBindings SmallFloat64::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("+", &SelfType::additionWith),
        makeMethodBinding("-", &SelfType::subtractionWith),
        makeMethodBinding("*", &SelfType::multiplicationWith),
        makeMethodBinding("/", &SelfType::divisionWith),
    };
}

Oop SmallFloat64::additionWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromFloat64(self().decodeSmallFloat() + other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(self().decodeSmallFloat() + other.decodeSmallFloat());
    else if(other->isNumber())
        return Oop::fromFloat64(self().decodeSmallFloat() + other->asFloat64());

    return Super::additionWith(other);
}

Oop SmallFloat64::subtractionWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromFloat64(self().decodeSmallFloat() - other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(self().decodeSmallFloat() - other.decodeSmallFloat());
    else if(other->isNumber())
        return Oop::fromFloat64(self().decodeSmallFloat() - other->asFloat64());

    return Super::subtractionWith(other);
}

Oop SmallFloat64::multiplicationWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromFloat64(self().decodeSmallFloat() * other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(self().decodeSmallFloat() * other.decodeSmallFloat());
    else if(other->isNumber())
        return Oop::fromFloat64(self().decodeSmallFloat() * other->asFloat64());

    return Super::multiplicationWith(other);
}

Oop SmallFloat64::divisionWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromFloat64(self().decodeSmallFloat() / other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(self().decodeSmallFloat() / other.decodeSmallFloat());
    else if(other->isNumber())
        return Oop::fromFloat64(self().decodeSmallFloat() / other->asFloat64());

    return Super::divisionWith(other);
}

std::string SmallFloat64::asString() const
{
    std::ostringstream out;
    out << self().decodeSmallFloat() << std::ends;
    return out.str();
}

double SmallFloat64::asFloat64() const
{
    return self().decodeSmallFloat();
}

} // End of namespace nyast
