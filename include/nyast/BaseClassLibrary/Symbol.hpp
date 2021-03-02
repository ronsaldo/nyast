#ifndef NYAST_BASE_CLASS_LIBRARY_SYMBOL_HPP
#define NYAST_BASE_CLASS_LIBRARY_SYMBOL_HPP

#pragma once

#include "String.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Symbol : Subclass<String, Symbol>
{
    static constexpr char const __className__[] = "Symbol";

    static MethodCategories __instanceMethods__();
    
    std::string printString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_SYMBOL_HPP
