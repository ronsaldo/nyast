#ifndef NYAST_BASE_CLASS_LIBRARY_HASHED_COLLECTION_HPP
#define NYAST_BASE_CLASS_LIBRARY_HASHED_COLLECTION_HPP

#pragma once

#include "Collection.hpp"

namespace nyast
{

struct HashedCollection : Subclass<Collection, HashedCollection>
{
    static constexpr char const __className__[] = "HashedCollection";

    void initialize();
    void fullCheck();
    void grow();

    size_t tally;
    MemberOop array;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_HASHED_COLLECTION_HPP
