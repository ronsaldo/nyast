#ifndef NYAST_BASE_CLASS_LIBRARY_NUMBER_HPP
#define NYAST_BASE_CLASS_LIBRARY_NUMBER_HPP

#pragma once

#include "Magnitude.hpp"

namespace nyast
{

struct Number : Subclass<Magnitude, Number>
{
    static constexpr char const __className__[] = "Number";

    static MethodCategories __instanceMethods__();

    bool isNumber() const;

    Oop additionWith(Oop other);
    Oop subtractionWith(Oop other);
    Oop multiplicationWith(Oop other);
    Oop divisionWith(Oop other);
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_NUMBER_HPP
