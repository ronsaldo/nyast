#include "nyast/BaseClassLibrary/LargeNegativeInteger.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"

namespace nyast
{

static NativeClassRegistration<LargeNegativeInteger> largeNegativeIntegerClassRegistration;

Oop LargeNegativeInteger::createWithUnormalizedData(size_t dataSize, const uint8_t *data)
{
    (void)dataSize;
    (void)data;
    return Oop::nil();
}

int64_t LargeNegativeInteger::asInt64() const
{
    if(size() > sizeof(uint64_t))
        return Super::asInt64();

    uint64_t decodedValue = 0;
    // FIXME: Support the big endian variant.
    memcpy(&decodedValue, variableData(), size());
    if(decodedValue > -uint64_t(INT64_MIN))
        return Super::asInt64();

    return -int64_t(decodedValue);
}

double LargeNegativeInteger::asFloat64() const
{
    // FIXME: Implement this in a much more proper way.
    return double(asInt64());
}


} // End of namespace nyast
