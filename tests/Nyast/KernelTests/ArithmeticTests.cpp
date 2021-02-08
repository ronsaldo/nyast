#include "nyast/SmalltalkCompiler/Evaluator.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace nyast;
using namespace nyast::SmalltalkCompiler;

SUITE(ArithmeticTests)
{
    TEST(Addition)
    {
        CHECK_EQUAL(3, evaluateDoItString("1 + 2")->asInt32());
        CHECK_EQUAL(0, evaluateDoItString("1 + -1")->asInt32());
        CHECK_EQUAL(3.0, evaluateDoItString("1.0 + 2.0")->asFloat64());
        CHECK_EQUAL(3.0, evaluateDoItString("1.5 + 1.5")->asFloat64());
        CHECK_EQUAL(0.0, evaluateDoItString("1.0 + -1.0")->asFloat64());
    }

    TEST(Subtraction)
    {
        CHECK_EQUAL(-1, evaluateDoItString("1 - 2")->asInt32());
        CHECK_EQUAL(2, evaluateDoItString("1 - -1")->asInt32());
        CHECK_EQUAL(-1.0, evaluateDoItString("1.0 - 2.0")->asFloat64());
        CHECK_EQUAL(0.0, evaluateDoItString("1.5 - 1.5")->asFloat64());
        CHECK_EQUAL(2.0, evaluateDoItString("1.0 - -1.0")->asFloat64());
    }

    TEST(Multiplication)
    {
        CHECK_EQUAL(2, evaluateDoItString("1 * 2")->asInt32());
        CHECK_EQUAL(-1, evaluateDoItString("1 * -1")->asInt32());
        CHECK_EQUAL(2.0, evaluateDoItString("1.0 * 2.0")->asFloat64());
        CHECK_EQUAL(2.25, evaluateDoItString("1.5 * 1.5")->asFloat64());
        CHECK_EQUAL(-1.0, evaluateDoItString("1.0 * -1.0")->asFloat64());
    }

    TEST(Division)
    {
        CHECK_EQUAL(0.5, evaluateDoItString("1 / 2")->asFloat64());
        CHECK_EQUAL(-1, evaluateDoItString("1 / -1")->asInt32());
        CHECK_EQUAL(0.5, evaluateDoItString("1.0 / 2.0")->asFloat64());
        CHECK_EQUAL(1.0, evaluateDoItString("1.5 / 1.5")->asFloat64());
        CHECK_EQUAL(-1.0, evaluateDoItString("1.0 / -1.0")->asFloat64());
    }
}
