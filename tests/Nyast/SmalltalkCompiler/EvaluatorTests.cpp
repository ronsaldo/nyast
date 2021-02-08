#include "nyast/SmalltalkCompiler/Evaluator.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace nyast;
using namespace nyast::SmalltalkCompiler;

SUITE(SmalltalkEvaluator)
{
    TEST(Nil)
    {
        CHECK_EQUAL(Oop::nil(), evaluateDoItString("nil"));
    }

    TEST(True)
    {
        CHECK_EQUAL(Oop::trueValue(), evaluateDoItString("true"));
        CHECK(evaluateDoItString("true")->asBoolean8());
    }

    TEST(False)
    {
        CHECK_EQUAL(Oop::falseValue(), evaluateDoItString("false"));
        CHECK(!evaluateDoItString("false")->asBoolean8());
    }

    TEST(LiteralInteger)
    {
        CHECK_EQUAL(-1, evaluateDoItString("-1")->asInt32());
        CHECK_EQUAL(0, evaluateDoItString("0")->asInt32());
        CHECK_EQUAL(1, evaluateDoItString("1")->asInt32());
        CHECK_EQUAL(42, evaluateDoItString("42")->asInt32());

        CHECK_EQUAL(0u, evaluateDoItString("0")->asUInt32());
        CHECK_EQUAL(1u, evaluateDoItString("1")->asUInt32());
        CHECK_EQUAL(42u, evaluateDoItString("42")->asUInt32());
    }

    TEST(LiteralFloat)
    {
        CHECK_EQUAL(42.5, evaluateDoItString("42.5")->asFloat64());
    }

    TEST(LiteralCharacter)
    {
        CHECK_EQUAL(char32_t('A'), evaluateDoItString("$A")->asChar32());
    }

    TEST(LiteralString)
    {
        CHECK_EQUAL("", evaluateDoItString("''")->asString());
        CHECK_EQUAL("Hello World", evaluateDoItString("'Hello World'")->asString());
        CHECK_EQUAL("Hello World\r\n", evaluateDoItString("'Hello World\r\n'")->asString());
        CHECK_EQUAL("'", evaluateDoItString("''''")->asString());
    }

    TEST(LiteralSymbol)
    {
        CHECK_EQUAL("test", evaluateDoItString("#test")->asString());
        CHECK_EQUAL("<", evaluateDoItString("#<")->asString());
        CHECK_EQUAL("hello:", evaluateDoItString("#hello:")->asString());
        CHECK_EQUAL("hello:world:", evaluateDoItString("#hello:world:")->asString());
    }

    TEST(LiteralSymbolString)
    {
        CHECK_EQUAL("", evaluateDoItString("#''")->asString());
        CHECK_EQUAL("Hello World", evaluateDoItString("#'Hello World'")->asString());
        CHECK_EQUAL("Hello World\r\n", evaluateDoItString("#'Hello World\r\n'")->asString());
        CHECK_EQUAL("'", evaluateDoItString("#''''")->asString());
    }

    TEST(LiteralArray)
    {
        CHECK_EQUAL(OopList{}, evaluateDoItString("#()")->asOopList());
        CHECK_EQUAL(OopList{Oop::fromInt32(42)}, evaluateDoItString("#(42)")->asOopList());
        CHECK_EQUAL((OopList{Oop::trueValue(), Oop::fromInt32(42)}), evaluateDoItString("#(true 42)")->asOopList());
    }

    TEST(LiteralByteArray)
    {
        CHECK(ByteArrayData{} == evaluateDoItString("#[]")->asByteArrayData());
        CHECK(ByteArrayData{0} == evaluateDoItString("#[0]")->asByteArrayData());
        CHECK((ByteArrayData{0, 1}) == evaluateDoItString("#[0 1]")->asByteArrayData());
    }

    TEST(Yourself)
    {
        CHECK_EQUAL(Oop::nil(), evaluateDoItString("nil yourself"));
        CHECK_EQUAL(Oop::trueValue(), evaluateDoItString("true yourself"));
        CHECK_EQUAL(Oop::falseValue(), evaluateDoItString("false yourself"));
        CHECK_EQUAL(-1, evaluateDoItString("-1 yourself")->asInt32());
        CHECK_EQUAL(0, evaluateDoItString("0 yourself")->asInt32());
        CHECK_EQUAL(1, evaluateDoItString("1 yourself")->asInt32());
        CHECK_EQUAL(42, evaluateDoItString("42 yourself")->asInt32());
    }
}
