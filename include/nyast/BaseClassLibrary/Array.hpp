#ifndef NYAST_BASE_CLASS_LIBRARY_ARRAY_HPP
#define NYAST_BASE_CLASS_LIBRARY_ARRAY_HPP

#pragma once

#include "ArrayedCollection.hpp"

namespace nyast
{

struct Array : SubclassWithVariableDataOfType<ArrayedCollection, Array, Oop>
{
    static constexpr char const __className__[] = "Array";

    static MethodBindings __instanceMethods__();

    bool isArray() const;
    
    std::vector<Oop> asOopList() const;
    std::string asString() const;
    std::string printString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_ARRAY_HPP
