#include "nyast/BaseClassLibrary/UndefinedObject.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
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

static NativeClassRegistration<UndefinedObject> undefinedObjectClassRegistration;

MethodCategories UndefinedObject::__instanceMethods__()
{
    return MethodCategories{
        {"printing", {
            makeMethodBinding("asString", &SelfType::asString),
        }}
    };
}

Oop UndefinedObject::evaluateValue() const
{
    return self();
}

std::string UndefinedObject::asString() const
{
    return "nil";
}

} // End of namespace nyast
