#ifndef NYAST_BASE_CLASS_LIBRARY_SMALL_INTEGER_HPP
#define NYAST_BASE_CLASS_LIBRARY_SMALL_INTEGER_HPP

#pragma once

#include "Integer.hpp"

namespace nyast
{

struct SmallInteger : SubclassWithImmediateRepresentation<Integer, SmallInteger>
{
    static constexpr char const __className__[] = "SmallInteger";

    static MethodCategories __instanceMethods__();

    std::string asString() const;

    uint32_t asUInt32() const;
    int32_t asInt32() const;
    uint64_t asUInt64() const;
    int64_t asInt64() const;
    double asFloat64() const;

    Oop additionWith(Oop other);
    Oop subtractionWith(Oop other);
    Oop multiplicationWith(Oop other);
    Oop divisionWith(Oop other);
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_SMALL_INTEGER_HPP
