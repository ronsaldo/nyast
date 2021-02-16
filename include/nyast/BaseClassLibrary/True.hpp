#ifndef NYAST_BASE_CLASS_LIBRARY_TRUE_HPP
#define NYAST_BASE_CLASS_LIBRARY_TRUE_HPP

#pragma once

#include "Boolean.hpp"

namespace nyast
{

struct True : Subclass<Boolean, True>
{
    static constexpr char const __className__[] = "True";

    static MethodCategories __instanceMethods__();
    
    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<True>::value();
    }


    bool asBoolean8() const;

    std::string asString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_TRUE_HPP
