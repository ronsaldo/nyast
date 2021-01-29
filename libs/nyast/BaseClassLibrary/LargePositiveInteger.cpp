#include "nyast/BaseClassLibrary/LargePositiveInteger.hpp"

namespace nyast
{

Oop LargePositiveInteger::createWithUnormalizedData(size_t dataSize, const uint8_t *data)
{
    (void)dataSize;
    (void)data;
    return Oop::nil();
}

uint64_t LargePositiveInteger::asUInt64() const
{
    if(size() > sizeof(uint64_t))
        return Super::asUInt64();

    uint64_t decodedValue = 0;
    // FIXME: Support the big endian variant.
    memcpy(&decodedValue, variableData(), size());
    return decodedValue;
}

int64_t LargePositiveInteger::asInt64() const
{
    auto value = asUInt64();
    if(value > uint64_t(INT64_MAX))
        return Super::asInt64();

    return int64_t(value);
}

double LargePositiveInteger::asFloat64() const
{
    // FIXME: Implement this in a much more proper way.
    return double(asUInt64());
}

} // End of namespace nyast
