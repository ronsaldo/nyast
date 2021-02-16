#ifndef NYAST_BASE_CLASS_LIBRARY_LOOKUP_KEY_HPP
#define NYAST_BASE_CLASS_LIBRARY_LOOKUP_KEY_HPP

#pragma once

#include "Magnitude.hpp"

namespace nyast
{

struct LookupKey : Subclass<Magnitude, LookupKey>
{
    static constexpr char const __className__[] = "LookupKey";

    static MethodCategories __instanceMethods__();
    static MethodCategories __classMethods__();
    static SlotDefinitions __slots__();

    LookupKey() = default;
    LookupKey(Oop ckey)
        : key(ckey) {}

    Oop getKey() const;

    MemberOop key;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP
