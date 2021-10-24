#include "nyast/BaseClassLibrary/Fraction.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{

static NativeClassRegistration<Fraction> fractionClassRegistration;

SlotDefinitions Fraction::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("numerator", &SelfType::numerator),
        makeMemberSlot("denominator", &SelfType::denominator),
    };
}

Oop Fraction::constructWithNumeratorDenominator(Oop numerator, Oop denominator)
{
    auto result = staticBasicNewInstance<SelfType> ();
    result->numerator = numerator;
    result->denominator = denominator;
    return Oop::fromObjectPtr(result);
}

double Fraction::asFloat64()
{
    return numerator->asFloat64() / denominator->asFloat64();
}

} // End of namespace nyast
