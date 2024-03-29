#ifndef NYAST_BASE_CLASS_LIBRARY_CPP_MEMBER_SLOT_HPP
#define NYAST_BASE_CLASS_LIBRARY_CPP_MEMBER_SLOT_HPP

#pragma once

#include "Slot.hpp"

namespace nyast
{

/**
 * I am the base class for a cpp functional method.
 */
struct NYAST_CORE_EXPORT CppMemberSlotBase : Subclass<Slot, CppMemberSlotBase>
{
    static constexpr char const __className__[] = "CppMemberSlot";

    static MethodCategories __instanceMethods__();
    static SlotDefinitions __slots__();

    std::string printString() const;
    std::string asString() const;
    
    void storeReferenceTypesInGCLayout(Oop gcLayout);

    size_t offset;
    size_t fieldSize;
    GCReferenceType referenceType;
};

template<typename MT>
struct CppMemberSlot : CppMemberSlotBase
{
    typedef MT MemberType;
    typedef typename CppMemberToAccessorArgumentType<MemberType>::type ArgumentType;

    MemberType *memberPointerForOop(Oop self) const
    {
        return reinterpret_cast<MemberType*> (self.value + offset);
    }

    Oop read(Oop self)
    {
        return CppToOop<MemberType> ()(*memberPointerForOop(self));
    }

    Oop writeTo(Oop value, Oop self)
    {
        *memberPointerForOop(self) = OopToCpp<ArgumentType> ()(value);
        return value;
    }
};

template<typename SelfType, typename MemberType>
size_t offsetOfMemberPointer(MemberType SelfType::*fieldPointer)
{
    return reinterpret_cast<size_t> (&(reinterpret_cast<SelfType*> (0)->*fieldPointer));
}

template<typename SelfType, typename MemberType>
SlotDefinition makeMemberSlot(const std::string &name, MemberType SelfType::*member)
{
    auto slot = staticBasicNewInstance<CppMemberSlot<MemberType>> ();
    slot->name = Oop::internSymbol(name);
    slot->offset = offsetOfMemberPointer(member);
    slot->fieldSize = sizeof(MemberType);
    slot->referenceType = GCReferenceTypeForFieldType<MemberType>::value;
    return Oop::fromObjectPtr(slot);
}

} // End of namespace nyast

#endif // NYAST_BASE_CLASS_LIBRARY_CPP_MEMBER_SLOT_HPP
