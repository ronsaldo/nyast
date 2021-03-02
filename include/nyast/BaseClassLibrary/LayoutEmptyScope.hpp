#ifndef NYAST_LAYOUT_EMPTY_SCOPE_HPP
#define NYAST_LAYOUT_EMPTY_SCOPE_HPP

#pragma once

#include "LayoutAbstractScope.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT LayoutEmptyScope : Subclass<LayoutAbstractScope, LayoutEmptyScope>
{
    static constexpr char const __className__[] = "LayoutEmptyScope";

    static MethodCategories __instanceMethods__();
};

} // End of namespace namespace nyast

#endif //NYAST_LAYOUT_EMPTY_SCOPE_HPP
