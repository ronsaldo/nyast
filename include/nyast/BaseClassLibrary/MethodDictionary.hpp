#ifndef NYAST_BASE_CLASS_LIBRARY_METHOD_DICTIONARY_HPP
#define NYAST_BASE_CLASS_LIBRARY_METHOD_DICTIONARY_HPP

#pragma once

#include "Dictionary.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT MethodDictionary : Subclass<Dictionary, MethodDictionary>
{
    static constexpr char const __className__[] = "MethodDictionary";

    static MethodCategories __instanceMethods__();
    static SlotDefinitions __slots__();

    Oop initialize();

    Oop add(Oop anAssociation);
    Oop scanFor(Oop key) const;
    Oop atOrNil(Oop key) const;

    Oop atPut(Oop key, Oop value);
    void grow();

    MemberOop methods;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_METHOD_DICTIONARY_HPP
