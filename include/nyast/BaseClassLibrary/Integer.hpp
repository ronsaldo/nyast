#ifndef NYAST_BASE_CLASS_LIBRARY_INTEGER_HPP
#define NYAST_BASE_CLASS_LIBRARY_INTEGER_HPP

#pragma once

#include "Number.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Integer : Subclass<Number, Integer>
{
    static constexpr char const __className__[] = "Integer";

    static MethodCategories __instanceMethods__();
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_INTEGER_HPP
