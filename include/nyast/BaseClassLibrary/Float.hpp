#ifndef NYAST_BASE_CLASS_LIBRARY_FLOAT_HPP
#define NYAST_BASE_CLASS_LIBRARY_FLOAT_HPP

#pragma once

#include "Number.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Float : Subclass<Number, Float>
{
    static constexpr char const __className__[] = "Float";
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_FLOAT_HPP
