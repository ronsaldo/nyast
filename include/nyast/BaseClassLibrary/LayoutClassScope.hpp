#ifndef NYAST_LAYOUT_CLASS_SCOPE_HPP
#define NYAST_LAYOUT_CLASS_SCOPE_HPP

#pragma once

#include "LayoutAbstractScope.hpp"

namespace nyast
{

struct LayoutClassScope : Subclass<LayoutAbstractScope, LayoutClassScope>
{
    static constexpr char const __className__[] = "LayoutClassScope";

    static MethodCategories __instanceMethods__();
    static SlotDefinitions __slots__();

    void setSlotDefinitions(const SlotDefinitions &slotDefinitions);

    MemberOop parentScope;
    MemberOop slots;
    MemberOop slotDictionary;
};

} // End of namespace namespace nyast

#endif //NYAST_LAYOUT_CLASS_SCOPE_HPP
