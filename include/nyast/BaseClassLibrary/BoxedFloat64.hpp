#ifndef NYAST_BASE_CLASS_LIBRARY_BOXED_FLOAT64_HPP
#define NYAST_BASE_CLASS_LIBRARY_BOXED_FLOAT64_HPP

#pragma once

#include "Float.hpp"

namespace nyast
{

struct BoxedFloat64 : Subclass<Float, BoxedFloat64>
{
    static constexpr char const __className__[] = "BoxedFloat64";

    static MethodCategories __instanceMethods__();

    std::string asString() const;
    double asFloat64() const;

    Oop additionWith(Oop other);
    Oop subtractionWith(Oop other);
    Oop multiplicationWith(Oop other);
    Oop divisionWith(Oop other);

    double value;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_BOXED_FLOAT64_HPP
