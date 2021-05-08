#include "nyast/BaseClassLibrary/Class.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{

static NativeClassRegistration<Class> classClassRegistration;

SlotDefinitions Class::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("subclasses", &SelfType::subclasses),
        makeMemberSlot("name", &SelfType::name),
        makeMemberSlot("metaClass", &SelfType::metaClass),
    };
}

MethodCategories Class::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeGetterMethodBinding("name", &SelfType::name),
            makeGetterMethodBinding("getName", &SelfType::name),
            makeSetterMethodBinding("setName:", &SelfType::name),
        }},

        {"printing", {
            makeMethodBinding("asString", &SelfType::asString),
        }},

        {"class hierarchy", {
            makeMethodBinding("addSubclass:", &SelfType::addSubclass),
            makeMethodBinding("subclasses", &SelfType::getSubclasses),
        }},
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
