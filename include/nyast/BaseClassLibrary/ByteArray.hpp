#ifndef NYAST_BASE_CLASS_LIBRARY_BYTE_ARRAY_HPP
#define NYAST_BASE_CLASS_LIBRARY_BYTE_ARRAY_HPP

#pragma once

#include "ArrayedCollection.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT ByteArray : SubclassWithVariableDataOfType<ArrayedCollection, ByteArray, uint8_t>
{
    static constexpr char const __className__[] = "ByteArray";

    ByteArrayData asByteArrayData() const;
    std::string asString() const;
    std::string printString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_ARRAY_HPP
