#ifndef NYAST_BASE_CLASS_LIBRARY_SEQUENCEABLE_COLLECTION_HPP
#define NYAST_BASE_CLASS_LIBRARY_SEQUENCEABLE_COLLECTION_HPP

#pragma once

#include "Collection.hpp"

namespace nyast
{

struct SequenceableCollection : Subclass<Collection, SequenceableCollection>
{
    static constexpr char const __className__[] = "SequenceableCollection";
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_BOOLEAN_HPP
