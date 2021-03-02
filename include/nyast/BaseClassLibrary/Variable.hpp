#ifndef NYAST_BASE_CLASS_LIBRARY_VARIABLE_HPP
#define NYAST_BASE_CLASS_LIBRARY_VARIABLE_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Variable : Subclass<Object, Variable>
{
    static constexpr char const __className__[] = "Variable";
    static MethodCategories __instanceMethods__();
    static SlotDefinitions __slots__();

    Oop getName() const;

    MemberOop name;
};

} // End of namespace nyast

#endif // NYAST_BASE_CLASS_LIBRARY_VARIABLE_HPP
