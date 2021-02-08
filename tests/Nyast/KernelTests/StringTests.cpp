#include "nyast/SmalltalkCompiler/Evaluator.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace nyast;
using namespace nyast::SmalltalkCompiler;

SUITE(StringTests)
{
    TEST(Equality)
    {
        CHECK(evaluateDoItString("'test' = 'test'")->asBoolean8());
        CHECK(evaluateDoItString("'test' hash = 'test' hash")->asBoolean8());
        CHECK(!evaluateDoItString("'test' ~= 'test'")->asBoolean8());
        CHECK(!evaluateDoItString("'test' hash ~= 'test' hash")->asBoolean8());

        CHECK(evaluateDoItString("'test' = #'test'")->asBoolean8());
        CHECK(evaluateDoItString("'test' hash = #'test' hash")->asBoolean8());
        CHECK(!evaluateDoItString("'test' ~= #'test'")->asBoolean8());
        CHECK(!evaluateDoItString("'test' hash ~= #'test' hash")->asBoolean8());
    }
}
