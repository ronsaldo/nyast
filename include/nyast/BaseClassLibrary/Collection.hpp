#ifndef NYAST_BASE_CLASS_LIBRARY_COLLECTION_HPP
#define NYAST_BASE_CLASS_LIBRARY_COLLECTION_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{
struct NYAST_CORE_EXPORT Collection : Subclass<Object, Collection>
{
    static constexpr char const __className__[] = "Collection";

    static MethodCategories __instanceMethods__();
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_BOOLEAN_HPP
