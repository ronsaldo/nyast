#include "nyast/BaseClassLibrary/Object.hpp"
#include "nyast/BaseClassLibrary/Association.hpp"
#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

static NativeClassRegistration<Object> objectClassRegistration;

MethodCategories Object::__instanceMethods__()
{
    return MethodCategories{
        {"associating", {
            makeMethodBinding<Oop (Oop, Oop)> ("->", +[](Oop self, Oop value) {
                return Oop::fromObjectPtr(staticBasicNewInstance<Association> (0, self, value));
            }),
        }},

        {"converting", {
            makeMethodBinding("asString", &SelfType::asString),
            makeMethodBinding("printString", &SelfType::printString),
        }},

        {"testing", {
            makeMethodBinding("isArray", &SelfType::isArray),
            makeMethodBinding("isAssociation", &SelfType::isAssociation),
            makeMethodBinding("isBehavior", &SelfType::isBehavior),
            makeMethodBinding("isBlock", &SelfType::isBlock),
            makeMethodBinding("isCharacter", &SelfType::isCharacter),
            makeMethodBinding("isDictionary", &SelfType::isDictionary),
            makeMethodBinding("isFloat", &SelfType::isFloat),
            makeMethodBinding("isFraction", &SelfType::isFraction),
            makeMethodBinding("isInteger", &SelfType::isInteger),
            makeMethodBinding("isInterval", &SelfType::isInterval),
            makeMethodBinding("isNumber", &SelfType::isNumber),
            makeMethodBinding("isString", &SelfType::isString),
            makeMethodBinding("isSymbol", &SelfType::isSymbol),
        }},

        {"errors", {
            makeMethodBinding("error", static_cast<Oop(SelfType::*)()> (&SelfType::error)),
            makeMethodBinding("error", static_cast<Oop(SelfType::*)(const std::string &)> (&SelfType::error)),
            makeMethodBinding("explicitRequirement", &SelfType::explicitRequirement),
            makeMethodBinding("subclassResponsibility", &SelfType::subclassResponsibility),
            makeMethodBinding("shouldBeImplemented", &SelfType::shouldBeImplemented),
            makeMethodBinding("shouldNotImplement", &SelfType::shouldNotImplement),
        }},
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

// Converting methods.
static bool isVowel(char c)
{
    switch(c)
    {
    case 'a': return true;
    case 'A': return true;
    case 'e': return true;
    case 'E': return true;
    case 'i': return true;
    case 'I': return true;
    case 'o': return true;
    case 'O': return true;
    case 'u': return true;
    case 'U': return true;
    default: return false;
    }
}

std::string Object::asString() const
{
    auto className = self().getClass()->asString();
    if(!className.empty() && isVowel(className.front()))
        return "an " + className;
    return "a " + className;
}

std::string Object::printString() const
{
    return self()->asString();
}

} // End of namespace nyast
