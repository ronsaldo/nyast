#ifndef NYAST_BASE_CLASS_LIBRARY_SEQUENCEABLE_COLLECTION_HPP
#define NYAST_BASE_CLASS_LIBRARY_SEQUENCEABLE_COLLECTION_HPP

#pragma once

#include "Collection.hpp"

namespace nyast
{

struct SequenceableCollection : Subclass<Collection, SequenceableCollection>
{
    static constexpr char const __className__[] = "SequenceableCollection";

    static MethodCategories __instanceMethods__();

    Oop first() const;
    Oop second() const;
    Oop third() const;
    Oop fourth() const;
    Oop fifth() const;
    Oop sixth() const;
    Oop seventh() const;
    Oop eighth() const;
    Oop ninth() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_BOOLEAN_HPP
