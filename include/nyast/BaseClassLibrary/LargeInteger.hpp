#ifndef NYAST_BASE_CLASS_LIBRARY_LARGE_INTEGER_HPP
#define NYAST_BASE_CLASS_LIBRARY_LARGE_INTEGER_HPP

#pragma once

#include "Integer.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT LargeInteger : SubclassWithVariableDataOfType<Integer, LargeInteger, uint8_t>
{
    static constexpr char const __className__[] = "LargeInteger";

    static Oop createWithSignAndUnormalizedData(bool isNegative, size_t dataSize, const uint8_t *data);
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_LARGE_INTEGER_HPP
