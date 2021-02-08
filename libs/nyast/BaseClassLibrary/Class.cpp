#include "nyast/BaseClassLibrary/Class.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

MethodBindings Class::__instanceMethods__()
{
    return MethodBindings{
        // Accessing
        makeGetterMethodBinding("name", &SelfType::name),
        makeGetterMethodBinding("getName", &SelfType::name),
        makeSetterMethodBinding("setName:", &SelfType::name),

        // Printing.
        makeMethodBinding("asString", &SelfType::asString),

        // Class hierarchy.
        makeMethodBinding("addSubclass:", &SelfType::addSubclass),
        makeMethodBinding("subclasses", &SelfType::getSubclasses),
    };
}

void Class::addSubclass(Oop subclass)
{
    OopList newSubclasses;
    if(subclasses.isNotNil())
        newSubclasses = subclasses->asOopList();
    newSubclasses.push_back(subclass);
    subclasses = Oop::fromOopList(newSubclasses);
}

Oop Class::getSubclasses()
{
    if(subclasses.isNil())
        subclasses = Oop::fromOopList(OopList{});
    return subclasses;
}

Oop Class::getClass() const
{
    return metaClass;
}

std::string Class::asString() const
{
    return name->asString();
}

} // End of namespace nyast
