#ifndef NYAST_BASE_CLASS_LIBRARY_VARIABLE_HPP
#define NYAST_BASE_CLASS_LIBRARY_VARIABLE_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct Variable : Subclass<Object, Variable>
{
    static constexpr char const __className__[] = "Variable";
    static MethodBindings __instanceMethods__();
    static SlotDefinitions __slots__();

    MemberOop name;
};

} // End of namespace nyast

#endif // NYAST_BASE_CLASS_LIBRARY_VARIABLE_HPP
