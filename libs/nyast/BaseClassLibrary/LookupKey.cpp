#include "nyast/BaseClassLibrary/LookupKey.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{

static NativeClassRegistration<LookupKey> lookupKeyClassRegistration;

SlotDefinitions LookupKey::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("key", &SelfType::key),
    };
}

MethodBindings LookupKey::__classMethods__()
{
    return MethodBindings{
        makeMethodBinding<Oop (Oop, Oop)> ("key:", [](Oop clazz, Oop key) {
            auto result = clazz->basicNewInstance();
            result.as<SelfType> ()->key = key;
            return result;
        }),
    };
}

MethodBindings LookupKey::__instanceMethods__()
{
    return MethodBindings{

        // Accessing
        makeGetterMethodBinding("key", &SelfType::key),
        makeSetterMethodBinding("key:", &SelfType::key),
    };
}


Oop LookupKey::getKey() const
{
    return key;
}

} // End of namespace nyast
