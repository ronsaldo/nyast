#ifndef NYAST_BASE_CLASS_LIBRARY_SYMBOL_HPP
#define NYAST_BASE_CLASS_LIBRARY_SYMBOL_HPP

#pragma once

#include "String.hpp"

namespace nyast
{

struct Symbol : Subclass<String, Symbol>
{
    static constexpr char const __className__[] = "Symbol";

    std::string printString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_SYMBOL_HPP
