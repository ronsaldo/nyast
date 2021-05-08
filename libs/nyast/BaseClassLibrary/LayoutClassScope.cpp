#include "nyast/BaseClassLibrary/LayoutClassScope.hpp"
#include "nyast/BaseClassLibrary/Dictionary.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{

static NativeClassRegistration<LayoutClassScope> layoutClassScopeClassRegistration;

SlotDefinitions LayoutClassScope::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("parentScope", &SelfType::parentScope),
        makeMemberSlot("slots", &SelfType::slots),
        makeMemberSlot("slotDictionary", &SelfType::slotDictionary),
    };
}

MethodCategories LayoutClassScope::__instanceMethods__()
{
    return MethodCategories{
            // Accessing
            {"accessing", {
                makeGetterMethodBinding("parentScope", &SelfType::parentScope),
                makeGetterMethodBinding("slots", &SelfType::slots),
            }},

    };
}

void LayoutClassScope::setSlotDefinitions(const SlotDefinitions &slotDefinitions)
{
    slots = Oop::fromOopList(slotDefinitions);
    slotDictionary = staticOopNewInstance<Dictionary> ();
    for(auto &slot : slotDefinitions)
        slotDictionary->atPut(slot->getName(), slot);
}


void LayoutClassScope::storeReferenceTypesInGCLayout(Oop gcLayout)
{
    parentScope->storeReferenceTypesInGCLayout(gcLayout);

    auto slotCount = slots->getSize();
    for(size_t i = 1; i <= slotCount; ++i)
    {
        auto slot = slots->at(Oop::fromUIntPtr(i));
        slot->storeReferenceTypesInGCLayout(gcLayout);
    }
}

} // End of namespace nyast
