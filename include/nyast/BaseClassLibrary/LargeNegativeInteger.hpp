#ifndef NYAST_BASE_CLASS_LIBRARY_LARGE_NEGATIVE_INTEGER_HPP
#define NYAST_BASE_CLASS_LIBRARY_LARGE_NEGATIVE_INTEGER_HPP

#pragma once

#include "LargeInteger.hpp"

namespace nyast
{

struct LargeNegativeInteger : Subclass<LargeInteger, LargeNegativeInteger>
{
    static constexpr char const __className__[] = "LargeNegativeInteger";

    static Oop createWithUnormalizedData(size_t dataSize, const uint8_t *data);

    int64_t asInt64() const;
    double asFloat64() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_LARGE_POSITIVE_INTEGER_HPP
