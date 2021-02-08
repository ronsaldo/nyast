#include "nyast/BaseClassLibrary/SmallInteger.hpp"
#include "nyast/BaseClassLibrary/LargeInteger.hpp"
#include "nyast/BaseClassLibrary/Fraction.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

MethodBindings SmallInteger::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("+", &SelfType::additionWith),
        makeMethodBinding("-", &SelfType::subtractionWith),
        makeMethodBinding("*", &SelfType::multiplicationWith),
        makeMethodBinding("/", &SelfType::divisionWith),
    };
}

std::string SmallInteger::asString() const
{
    std::ostringstream out;
    out << self().decodeSmallInteger() << std::ends;
    return out.str();
}

uint32_t SmallInteger::asUInt32() const
{
    static_assert(sizeof(uint32_t) <= sizeof(intptr_t));
    auto value = self().decodeSmallInteger();
    if(0 <= value && value <= intptr_t(UINT32_MAX))
        return uint32_t(value);

    return Super::asUInt32();
}

int32_t SmallInteger::asInt32() const
{
    static_assert(sizeof(int32_t) <= sizeof(intptr_t));
    auto value = self().decodeSmallInteger();
    if(INT32_MIN <= value && value <= INT32_MAX)
        return int32_t(value);

    return Super::asInt32();
}

uint64_t SmallInteger::asUInt64() const
{
    static_assert(sizeof(intptr_t) <= sizeof(uint64_t));
    auto value = self().decodeSmallInteger();
    if(value >= 0)
        return uint64_t(value);

    return Super::asUInt64();
}

int64_t SmallInteger::asInt64() const
{
    static_assert(sizeof(intptr_t) <= sizeof(int64_t));
    return self().decodeSmallInteger();
}

double SmallInteger::asFloat64() const
{
    return self().decodeSmallInteger();
}

Oop SmallInteger::additionWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromIntPtr(self().decodeSmallInteger() + other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(self().decodeSmallInteger() + other.decodeSmallFloat());
    else if(other->isFloat())
        return Oop::fromFloat64(self().decodeSmallInteger() + other->asFloat64());

    return Super::additionWith(other);
}

Oop SmallInteger::subtractionWith(Oop other)
{
    if(other.isSmallInteger())
        return Oop::fromIntPtr(self().decodeSmallInteger() - other.decodeSmallInteger());
    else if(other.isSmallFloat())
        return Oop::fromFloat64(self().decodeSmallInteger() - other.decodeSmallFloat());
    else if(other->isFloat())
        return Oop::fromFloat64(self().decodeSmallInteger() - other->asFloat64());

    return Super::subtractionWith(other);
}

Oop SmallInteger::multiplicationWith(Oop other)
{
    if(other.isSmallInteger())
    {
        auto selfValue = self().decodeSmallInteger();
        auto otherValue = other.decodeSmallInteger();

        auto directResult = selfValue * otherValue;
        if(selfValue == 0 || directResult / selfValue == otherValue)
            return Oop::fromIntPtr(directResult);

        uintptr_t selfAbs = selfValue >= 0 ? uintptr_t(selfValue) : uintptr_t(-selfValue);
        uintptr_t otherAbs = otherValue >= 0 ? uintptr_t(otherValue) : uintptr_t(-otherValue);
        bool resultIsNegative = (selfValue < 0) ^ (otherValue < 0);

        constexpr uintptr_t ShiftAmount = sizeof(uintptr_t)*4;
        constexpr uintptr_t LowMask = (uintptr_t(1)<<ShiftAmount) - 1;
        auto selfHigh = selfAbs >> ShiftAmount;
        auto selfLow = selfAbs & LowMask;

        auto otherHigh = otherAbs >> ShiftAmount;
        auto otherLow = otherAbs & LowMask;

        uintptr_t resultContent[2] = {
            selfLow*otherLow + ((selfLow*otherHigh + otherHigh*selfLow) << ShiftAmount),
            selfHigh*otherHigh
        };

        return LargeInteger::createWithSignAndUnormalizedData(resultIsNegative, sizeof(resultContent), reinterpret_cast<uint8_t*> (resultContent));
    }
    else if(other.isSmallFloat())
        return Oop::fromFloat64(self().decodeSmallInteger() * other.decodeSmallFloat());
    else if(other->isFloat())
        return Oop::fromFloat64(self().decodeSmallInteger() * other->asFloat64());

    return Super::multiplicationWith(other);
}

Oop SmallInteger::divisionWith(Oop other)
{
    if(other.isSmallInteger())
    {
        auto dividend = self().decodeSmallInteger();
        auto divisor = other.decodeSmallInteger();
        if(divisor == 0)
            return error("Division by zero");
        else if(dividend % divisor == 0)
            return Oop::fromIntPtr(dividend / divisor);
        else
            return Fraction::constructWithNumeratorDenominator(self(), other);
    }
    else if(other.isSmallFloat())
        return Oop::fromFloat64(self().decodeSmallInteger() / other.decodeSmallFloat());
    else if(other->isFloat())
        return Oop::fromFloat64(self().decodeSmallInteger() / other->asFloat64());

    return Super::divisionWith(other);
}

} // End of namespace nyast
