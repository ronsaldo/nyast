#include "nyast/SmalltalkCompiler/Evaluator.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace nyast;
using namespace nyast::SmalltalkCompiler;

SUITE(Associationtest)
{
    TEST(Creation)
    {
        CHECK(evaluateDoItString("Association new key = nil")->asBoolean8());
        CHECK(evaluateDoItString("Association new value = nil")->asBoolean8());

        CHECK(evaluateDoItString("(Association key: 1) key = 1")->asBoolean8());
        CHECK(evaluateDoItString("(Association key: 1) value = nil")->asBoolean8());

        CHECK(evaluateDoItString("(Association key: 1 value: 2) key = 1")->asBoolean8());
        CHECK(evaluateDoItString("(Association key: 1 value: 2) value = 2")->asBoolean8());

        CHECK(evaluateDoItString("(1 -> 2) key = 1")->asBoolean8());
        CHECK(evaluateDoItString("(1 -> 2) value = 2")->asBoolean8());
    }
}
