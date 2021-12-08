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

    TEST(Factorial)
    {
        CHECK_EQUAL("1", evaluateDoItString("0 factorial asString")->asStdString());
        CHECK_EQUAL("1", evaluateDoItString("1 factorial asString")->asStdString());
        CHECK_EQUAL("2", evaluateDoItString("2 factorial asString")->asStdString());
        CHECK_EQUAL("6", evaluateDoItString("3 factorial asString")->asStdString());
        CHECK_EQUAL("24", evaluateDoItString("4 factorial asString")->asStdString());
        CHECK_EQUAL("120", evaluateDoItString("5 factorial asString")->asStdString());
        CHECK_EQUAL("720", evaluateDoItString("6 factorial asString")->asStdString());
        CHECK_EQUAL("5040", evaluateDoItString("7 factorial asString")->asStdString());
        CHECK_EQUAL("40320", evaluateDoItString("8 factorial asString")->asStdString());
        CHECK_EQUAL("362880", evaluateDoItString("9 factorial asString")->asStdString());
        CHECK_EQUAL("3628800", evaluateDoItString("10 factorial asString")->asStdString());

        CHECK_EQUAL("2432902008176640000", evaluateDoItString("20 factorial asString")->asStdString());
        CHECK_EQUAL("265252859812191058636308480000000", evaluateDoItString("30 factorial asString")->asStdString());
        CHECK_EQUAL("815915283247897734345611269596115894272000000000", evaluateDoItString("40 factorial asString")->asStdString());
        CHECK_EQUAL("30414093201713378043612608166064768844377641568960512000000000000", evaluateDoItString("50 factorial asString")->asStdString());
        CHECK_EQUAL("93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000", evaluateDoItString("100 factorial asString")->asStdString());
    }
}
