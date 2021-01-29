#ifndef NYAST_BASE_CLASS_LIBRARY_MAGNITUDE_HPP
#define NYAST_BASE_CLASS_LIBRARY_MAGNITUDE_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct Magnitude : Subclass<Object, Magnitude>
{
    static constexpr char const __className__[] = "Magnitude";
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_MAGNITUDE_HPP
