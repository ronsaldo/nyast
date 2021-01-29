#include "nyast/BaseClassLibrary/UndefinedObject.hpp"

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

std::string UndefinedObject::asString() const
{
    return "nil";
}

} // End of namespace nyast
