#include "nyast/BaseClassLibrary/Fraction.hpp"

namespace nyast
{

Oop Fraction::constructWithNumeratorDenominator(Oop numerator, Oop denominator)
{
    auto result = basicNewInstance<SelfType> ();
    result->numerator = numerator;
    result->denominator = denominator;
    return Oop::fromObjectPtr(result);
}

double Fraction::asFloat64()
{
    return numerator->asFloat64() / denominator->asFloat64();
}

} // End of namespace nyast
