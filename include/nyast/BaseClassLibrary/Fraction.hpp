#ifndef NYAST_BASE_CLASS_LIBRARY_FRACTION_HPP
#define NYAST_BASE_CLASS_LIBRARY_FRACTION_HPP

#pragma once

#include "Number.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Fraction : Subclass<Number, Fraction>
{
    static constexpr char const __className__[] = "Fraction";

    static Oop constructWithNumeratorDenominator(Oop numerator, Oop denominator);

    double asFloat64();

    MemberOop numerator;
    MemberOop denominator;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_FRACTION_HPP
