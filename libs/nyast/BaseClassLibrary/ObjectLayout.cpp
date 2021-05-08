#include "nyast/BaseClassLibrary/ObjectLayout.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{

static NativeClassRegistration<ObjectLayout> objectLayoutClassRegistration;

SlotDefinitions ObjectLayout::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("slotScope", &SelfType::slotScope),
    };
}

MethodCategories ObjectLayout::__instanceMethods__()
{
    return MethodCategories{
        // Accessing
        {"accessing", {
            makeGetterMethodBinding("slotScope", &SelfType::slotScope),
        }},
    };
}

Oop ObjectLayout::getSlotScope() const
{
    return slotScope;
}

void ObjectLayout::storeReferenceTypesInGCLayout(Oop gcLayout)
{
    slotScope->storeReferenceTypesInGCLayout(gcLayout);
}

} // End of namespace nyast
