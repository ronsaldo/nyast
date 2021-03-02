#ifndef NYAST_BASE_CLASS_LIBRARY_STRING_HPP
#define NYAST_BASE_CLASS_LIBRARY_STRING_HPP

#pragma once

#include "ArrayedCollection.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT String : SubclassWithVariableDataOfType<ArrayedCollection, String, char>
{
    static constexpr char const __className__[] = "String";

    static MethodCategories __instanceMethods__();

    OopHash hash() const;
    bool equals(Oop other) const;

    std::string asStdString() const;
    std::string asString() const;
    std::string printString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_STRING_HPP
