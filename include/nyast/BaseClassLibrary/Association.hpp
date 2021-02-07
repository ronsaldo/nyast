#ifndef NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP
#define NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct Association : Subclass<Object, Association>
{
    static constexpr char const __className__[] = "Association";

    static MethodBindings __instanceMethods__();

    bool isAssociation() const;

    Oop getKey() const;
    Oop evaluateValue() const;

    MemberOop key;
    MemberOop value;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP
