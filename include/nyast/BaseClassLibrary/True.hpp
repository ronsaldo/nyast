#ifndef NYAST_BASE_CLASS_LIBRARY_TRUE_HPP
#define NYAST_BASE_CLASS_LIBRARY_TRUE_HPP

#pragma once

#include "Boolean.hpp"

namespace nyast
{

struct True : Subclass<Boolean, True>
{
    static constexpr char const __className__[] = "True";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<True>::value();
    }

    std::string asString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_TRUE_HPP
