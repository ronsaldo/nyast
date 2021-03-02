#ifndef NYAST_BASE_CLASS_LIBRARY_ARRAYED_COLLECTION_HPP
#define NYAST_BASE_CLASS_LIBRARY_ARRAYED_COLLECTION_HPP

#pragma once

#include "SequenceableCollection.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT ArrayedCollection : Subclass<SequenceableCollection, ArrayedCollection>
{
    static constexpr char const __className__[] = "ArrayedCollection";

    static MethodCategories __instanceMethods__();
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_ARRAYED_COLLECTION_HPP
