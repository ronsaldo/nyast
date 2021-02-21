#ifndef NYAST_ABSTRACT_LAYOUT_HPP
#define NYAST_ABSTRACT_LAYOUT_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct AbstractLayout : Subclass<Object, AbstractLayout>
{
    static constexpr char const __className__[] = "AbstractLayout";

    static MethodCategories __instanceMethods__();
    static SlotDefinitions __slots__();

    MemberOop host;
};

} // End of namespace namespace nyast

#endif //NYAST_ABSTRACT_LAYOUT_HPP
