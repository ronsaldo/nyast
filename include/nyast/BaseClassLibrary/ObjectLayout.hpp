#ifndef NYAST_OBJECT_LAYOUT_HPP
#define NYAST_OBJECT_LAYOUT_HPP

#pragma once

#include "AbstractLayout.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT ObjectLayout : Subclass<AbstractLayout, ObjectLayout>
{
    static constexpr char const __className__[] = "ObjectLayout";

    static MethodCategories __instanceMethods__();
    static SlotDefinitions __slots__();

    Oop getSlotScope() const;

    MemberOop slotScope;
};

} // End of namespace namespace nyast

#endif //NYAST_OBJECT_LAYOUT_HPP
