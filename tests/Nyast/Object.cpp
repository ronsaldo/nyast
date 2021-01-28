#include "nyast/Object.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace nyast;

SUITE(Object)
{
    TEST(Nil)
    {
        CHECK(Oop::nil().isNil());
        CHECK(!Oop::nil().isNotNil());
        CHECK_EQUAL("UndefinedObject", Oop::nil()->getClassName());
    }

    TEST(True)
    {
        CHECK(!Oop::trueValue().isNil());
        CHECK(Oop::trueValue().isNotNil());
        CHECK_EQUAL("True", Oop::trueValue()->getClassName());
    }

    TEST(False)
    {
        CHECK(!Oop::falseValue().isNil());
        CHECK(Oop::falseValue().isNotNil());
        CHECK_EQUAL("False", Oop::falseValue()->getClassName());
    }

    TEST(Character)
    {
        CHECK(Oop::fromChar32('A').isCharacter());
        CHECK_EQUAL(char32_t('A'), Oop::fromChar32('A').decodeCharacter());
        CHECK_EQUAL(Oop::fromChar32('A'), Oop::fromChar32('A'));
        CHECK_EQUAL("Character", Oop::fromChar32('A')->getClassName());
    }

    TEST(SmallInteger)
    {
        CHECK(Oop::fromInt32(0).isSmallInteger());
        CHECK_EQUAL(0, Oop::fromInt32(0).decodeSmallInteger());
        CHECK_EQUAL(Oop::fromInt32(0), Oop::fromInt32(0));
        CHECK_EQUAL("SmallInteger", Oop::fromInt32(0)->getClassName());

        CHECK(Oop::fromInt32(-1).isSmallInteger());
        CHECK_EQUAL(-1, Oop::fromInt32(-1).decodeSmallInteger());
        CHECK_EQUAL(Oop::fromInt32(-1), Oop::fromInt32(-1));
        CHECK_EQUAL("SmallInteger", Oop::fromInt32(-1)->getClassName());

        CHECK(Oop::fromInt32(1).isSmallInteger());
        CHECK_EQUAL(1, Oop::fromInt32(1).decodeSmallInteger());
        CHECK_EQUAL(Oop::fromInt32(1), Oop::fromInt32(1));
        CHECK_EQUAL("SmallInteger", Oop::fromInt32(1)->getClassName());
    }

    TEST(String)
    {
        auto oop = Oop::fromString("TestString");
        CHECK(oop.isPointer());
        CHECK(oop.isNotNil());
        CHECK_EQUAL("TestString", oop->asStdString());
        CHECK_EQUAL("TestString", oop->asString());
        CHECK_EQUAL("'TestString'", oop->printString());
        CHECK_EQUAL("String", oop->getClassName());
    }

    TEST(Symbol)
    {
        auto oop = Oop::internSymbol("TestSymbol");
        CHECK(oop.isPointer());
        CHECK(oop.isNotNil());
        CHECK_EQUAL("TestSymbol", oop->asStdString());
        CHECK_EQUAL("TestSymbol", oop->asString());
        CHECK_EQUAL("#'TestSymbol'", oop->printString());
        CHECK_EQUAL(oop, Oop::internSymbol("TestSymbol"));
        CHECK_EQUAL("Symbol", oop->getClassName());
    }

}
