#ifndef NYAST_BASE_CLASS_LIBRARY_METHOD_DICTIONARY_HPP
#define NYAST_BASE_CLASS_LIBRARY_METHOD_DICTIONARY_HPP

#pragma once

#include "Dictionary.hpp"

namespace nyast
{

struct MethodDictionary : Subclass<Dictionary, MethodDictionary>
{
    static constexpr char const __className__[] = "MethodDictionary";

    void initialize();

    Oop scanFor(Oop key) const;
    Oop atOrNil(Oop key) const;

    Oop atPut(Oop key, Oop value);
    void grow();

    Oop methods;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_METHOD_DICTIONARY_HPP