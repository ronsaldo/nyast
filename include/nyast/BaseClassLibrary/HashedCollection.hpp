#ifndef NYAST_BASE_CLASS_LIBRARY_HASHED_COLLECTION_HPP
#define NYAST_BASE_CLASS_LIBRARY_HASHED_COLLECTION_HPP

#pragma once

#include "Collection.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT HashedCollection : Subclass<Collection, HashedCollection>
{
    static constexpr char const __className__[] = "HashedCollection";

    static MethodCategories __instanceMethods__();

    Oop initialize();
    void fullCheck();
    void grow();

    size_t tally = 0;
    MemberOop array;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_HASHED_COLLECTION_HPP
