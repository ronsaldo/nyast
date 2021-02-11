#include "nyast/BaseClassLibrary/BoxedFloat64.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<BoxedFloat64> boxedFloat64ClassRegistration;

Oop Oop::makeBoxedFloat(double value)
{
    auto boxedFloat64 = newInstance<BoxedFloat64> ();
    boxedFloat64->value = value;
    return Oop::fromObjectPtr(boxedFloat64);
}

MethodBindings BoxedFloat64::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("+", &SelfType::additionWith),
        makeMethodBinding("-", &SelfType::subtractionWith),
        makeMethodBinding("*", &SelfType::multiplicationWith),
        makeMethodBinding("/", &SelfType::divisionWith),
    };
}

Oop BoxedFloat64::additionWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromFloat64(value + other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(value + other.decodeSmallFloat());
    else if(other->isNumber())
        return Oop::fromFloat64(value + other->asFloat64());

    return Super::additionWith(other);
}

Oop BoxedFloat64::subtractionWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromFloat64(value - other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(value - other.decodeSmallFloat());
    else if(other->isNumber())
        return Oop::fromFloat64(value - other->asFloat64());

    return Super::subtractionWith(other);
}

Oop BoxedFloat64::multiplicationWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromFloat64(value * other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(value * other.decodeSmallFloat());
    else if(other->isNumber())
        return Oop::fromFloat64(value * other->asFloat64());

    return Super::multiplicationWith(other);
}

Oop BoxedFloat64::divisionWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromFloat64(value / other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(value / other.decodeSmallFloat());
    else if(other->isNumber())
        return Oop::fromFloat64(value / other->asFloat64());

    return Super::divisionWith(other);
}

std::string BoxedFloat64::asString() const
{
    std::ostringstream out;
    out << value << std::ends;
    return out.str();
}

double BoxedFloat64::asFloat64() const
{
    return value;
}
} // End of namespace nyast
