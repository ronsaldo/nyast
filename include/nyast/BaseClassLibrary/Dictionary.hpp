#ifndef NYAST_BASE_CLASS_LIBRARY_DICTIONARY_HPP
#define NYAST_BASE_CLASS_LIBRARY_DICTIONARY_HPP

#pragma once

#include "HashedCollection.hpp"

namespace nyast
{

struct Dictionary : Subclass<HashedCollection, Dictionary>
{
    static constexpr char const __className__[] = "Dictionary";

    Oop atOrNil(Oop key) const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_DICTIONARY_HPP
