#include "nyast/BaseClassLibrary/Object.hpp"

namespace nyast
{

MethodBindings Object::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("error", static_cast<Oop(SelfType::*)()> (&SelfType::error)),
        makeMethodBinding("error", static_cast<Oop(SelfType::*)(const std::string &)> (&SelfType::error)),
        makeMethodBinding("explicitRequirement", &SelfType::explicitRequirement),
        makeMethodBinding("subclassResponsibility", &SelfType::subclassResponsibility),
        makeMethodBinding("shouldBeImplemented", &SelfType::shouldBeImplemented),
        makeMethodBinding("shouldNotImplement", &SelfType::shouldNotImplement),
    };
}

Oop Object::error(const std::string &errorMessage)
{
    throw std::runtime_error(errorMessage);
}

Oop Object::error()
{
    return error("Error!");
}

Oop Object::explicitRequirement()
{
    return error("Explicit requirement.");
}

Oop Object::subclassResponsibility()
{
    return error("Subclass responsibility.");
}

Oop Object::shouldBeImplemented()
{
    return error("Should be implemented.");
}

Oop Object::shouldNotImplement()
{
    return error("Should not implemented.");
}

bool Object::isArray() const
{
    return false;
}

bool Object::isAssociation() const
{
    return false;
}

bool Object::isBehavior() const
{
    return false;
}

bool Object::isBlock() const
{
    return false;
}

bool Object::isCharacter() const
{
    return false;
}

bool Object::isDictionary() const
{
    return false;
}

bool Object::isFloat() const
{
    return false;
}

bool Object::isFraction() const
{
    return false;
}

bool Object::isInteger() const
{
    return false;
}

bool Object::isInterval() const
{
    return false;
}

bool Object::isNumber() const
{
    return false;
}

bool Object::isString() const
{
    return false;
}

bool Object::isSymbol() const
{
    return false;
}

} // End of namespace nyast
