#include "nyast/BaseClassLibrary/LargeInteger.hpp"
#include "nyast/BaseClassLibrary/LargePositiveInteger.hpp"
#include "nyast/BaseClassLibrary/LargeNegativeInteger.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"

namespace nyast
{

static NativeClassRegistration<LargeInteger> largeIntegerClassRegistration;

Oop Oop::fromInt64(int64_t value)
{
    if(SmallIntegerMinValue <= value && value <= SmallIntegerMaxValue)
        return Oop{(uintptr_t(value) << SmallIntegerTagShift) | SmallIntegerTagValue};

    if(value < 0)
    {
        auto largeInteger = newInstance<LargeNegativeInteger> (8);
        auto absoluteValue = -value;
        memcpy(largeInteger->variableData(), &absoluteValue, 8);
        return Oop::fromObjectPtr(largeInteger);
    }
    else
    {
        auto largeInteger = newInstance<LargePositiveInteger> (8);
        memcpy(largeInteger->variableData(), &value, 8);
        return Oop::fromObjectPtr(largeInteger);
    }
}

Oop Oop::fromUInt64(uint64_t value)
{
    if(value <= uint64_t(SmallIntegerMaxValue))
        return Oop{(uintptr_t(value) << SmallIntegerTagShift) | SmallIntegerTagValue};

    auto largeInteger = newInstance<LargePositiveInteger> (8);
    memcpy(largeInteger->variableData(), &value, 8);
    return Oop::fromObjectPtr(largeInteger);
}

Oop LargeInteger::createWithSignAndUnormalizedData(bool isNegative, size_t dataSize, const uint8_t *data)
{
    return isNegative ? LargeNegativeInteger::createWithUnormalizedData(dataSize, data) : LargePositiveInteger::createWithUnormalizedData(dataSize, data);
}

} // End of namespace nyast
