#ifndef NYAST_BASE_CLASS_LIBRARY_DICTIONARY_HPP
#define NYAST_BASE_CLASS_LIBRARY_DICTIONARY_HPP

#pragma once

#include "HashedCollection.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Dictionary : Subclass<HashedCollection, Dictionary>
{
    static constexpr char const __className__[] = "Dictionary";

    static MethodCategories __instanceMethods__();

    Oop scanFor(Oop key) const;

    Oop add(Oop anAssociation);
    Oop atOrNil(Oop key) const;

    Oop atPut(Oop key, Oop value);
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_DICTIONARY_HPP
