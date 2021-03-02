#ifndef NYAST_BASE_CLASS_LIBRARY_LARGE_POSITIVE_INTEGER_HPP
#define NYAST_BASE_CLASS_LIBRARY_LARGE_POSITIVE_INTEGER_HPP

#pragma once

#include "LargeInteger.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT LargePositiveInteger : Subclass<LargeInteger, LargePositiveInteger>
{
    static constexpr char const __className__[] = "LargePositiveInteger";

    static Oop createWithUnormalizedData(size_t dataSize, const uint8_t *data);

    uint64_t asUInt64() const;
    int64_t asInt64() const;
    double asFloat64() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_LARGE_POSITIVE_INTEGER_HPP
