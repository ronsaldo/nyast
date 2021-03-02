#ifndef NYAST_LAYOUT_ABSTRACT_SCOPE_HPP
#define NYAST_LAYOUT_ABSTRACT_SCOPE_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT LayoutAbstractScope : Subclass<Object, LayoutAbstractScope>
{
    static constexpr char const __className__[] = "LayoutAbstractScope";

    static MethodCategories __instanceMethods__();
};

} // End of namespace namespace nyast

#endif //NYAST_LAYOUT_ABSTRACT_SCOPE_HPP
