#ifndef NYAST_BASE_CLASS_LIBRARY_SLOT_HPP
#define NYAST_BASE_CLASS_LIBRARY_SLOT_HPP

#pragma once

#include "Variable.hpp"

namespace nyast
{

struct Slot : Subclass<Variable, Slot>
{
    static constexpr char const __className__[] = "Slot";

    // Meta object protocol.
    Oop read(Oop receiver);
    Oop writeTo(Oop value, Oop receiver);
};

} // End of namespace nyast

#endif // NYAST_BASE_CLASS_LIBRARY_SLOT_HPP
