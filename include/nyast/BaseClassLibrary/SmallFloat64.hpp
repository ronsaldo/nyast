#ifndef NYAST_BASE_CLASS_LIBRARY_SMALL_FLOAT64_HPP
#define NYAST_BASE_CLASS_LIBRARY_SMALL_FLOAT64_HPP

#pragma once

#include "Float.hpp"

namespace nyast
{

struct SmallFloat64 : SubclassWithImmediateRepresentation<Float, SmallFloat64>
{
    static constexpr char const __className__[] = "SmallFloat64";

    static MethodBindings __instanceMethods__();

    std::string asString() const;
    double asFloat64() const;

    Oop additionWith(Oop other);
    Oop subtractionWith(Oop other);
    Oop multiplicationWith(Oop other);
    Oop divisionWith(Oop other);
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_SMALL_FLOAT64_HPP
