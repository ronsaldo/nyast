#ifndef NYAST_BASE_CLASS_LIBRARY_COLLECTION_HPP
#define NYAST_BASE_CLASS_LIBRARY_COLLECTION_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{
struct Collection : Subclass<Object, Collection>
{
    static constexpr char const __className__[] = "Collection";
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_BOOLEAN_HPP
