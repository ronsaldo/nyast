#ifndef NYAST_BASE_CLASS_LIBRARY_FALSE_HPP
#define NYAST_BASE_CLASS_LIBRARY_FALSE_HPP

#pragma once

#include "Boolean.hpp"

namespace nyast
{

struct False : Subclass<Boolean, False>
{
    static constexpr char const __className__[] = "False";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<False>::value();
    }

    std::string asString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_FALSE_HPP
