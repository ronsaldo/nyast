#ifndef NYAST_BASE_CLASS_LIBRARY_BOOLEAN_HPP
#define NYAST_BASE_CLASS_LIBRARY_BOOLEAN_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Boolean : Subclass<Object, Boolean>
{
    static constexpr char const __className__[] = "Boolean";
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_BOOLEAN_HPP
