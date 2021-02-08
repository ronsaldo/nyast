#include "nyast/BaseClassLibrary/UndefinedObject.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

Oop::Oop()
    : OopPointerSizeDependentImplementation{UndefinedObject::uniqueInstance().value}
{
}

Oop Oop::nil()
{
    return UndefinedObject::uniqueInstance();
}

MethodBindings UndefinedObject::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("asString", &SelfType::asString),
    };
}

std::string UndefinedObject::asString() const
{
    return "nil";
}

} // End of namespace nyast
