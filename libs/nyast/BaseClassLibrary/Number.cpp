#include "nyast/BaseClassLibrary/Number.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{
static NativeClassRegistration<Number> numberClassRegistration;

MethodBindings Number::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("+", &SelfType::additionWith),
        makeMethodBinding("-", &SelfType::subtractionWith),
        makeMethodBinding("*", &SelfType::multiplicationWith),
        makeMethodBinding("/", &SelfType::divisionWith),

        makeMethodBinding("isNumber", &SelfType::isNumber),
    };
}

bool Number::isNumber() const
{
    return true;
}

Oop Number::additionWith(Oop)
{
    return subclassResponsibility();
}

Oop Number::subtractionWith(Oop)
{
    return subclassResponsibility();
}

Oop Number::multiplicationWith(Oop)
{
    return subclassResponsibility();
}

Oop Number::divisionWith(Oop)
{
    return subclassResponsibility();
}

} // End of namespace nyast
