#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<CppMemberSlotBase> cppMemberSlotBaseClassRegistration;

SlotDefinitions CppMemberSlotBase::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("offset", &SelfType::offset),
        makeMemberSlot("fieldSize", &SelfType::fieldSize),
        makeMemberSlot("referenceType", &SelfType::referenceType),
    };
}

MethodCategories CppMemberSlotBase::__instanceMethods__()
{
    return MethodCategories{
        {"printing", {
            makeMethodBinding("printString", &SelfType::printString),
            makeMethodBinding("asString", &SelfType::asString),
        }},
    };
}

std::string CppMemberSlotBase::printString() const
{
    std::ostringstream out;
    out << "CppMemberSlot named: " << name->printString()
        << " offset: " << offset
        << " fieldSize: " << fieldSize
        << " referenceType: #" << GCReferenceTypeToString(referenceType) << std::ends;
    return out.str();
}

std::string CppMemberSlotBase::asString() const
{
    return self()->printString();
}

void CppMemberSlotBase::storeReferenceTypesInGCLayout(Oop gcLayout)
{
    gcLayout->setGCReferenceTypeAtOffsetSize(offset, fieldSize, referenceType);
}

} // End of namespace nyast
