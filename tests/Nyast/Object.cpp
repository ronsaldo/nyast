#include "nyast/Object.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace nyast;

SUITE(NIL)
{
    TEST(Nil)
    {
        CHECK(Oop::nil().isNil());
        CHECK(!Oop::nil().isNotNil());
    }

    TEST(Character)
    {
        CHECK(Oop::fromChar32('A').isCharacter());
        CHECK_EQUAL(char32_t('A'), Oop::fromChar32('A').decodeCharacter());
        CHECK_EQUAL(Oop::fromChar32('A'), Oop::fromChar32('A'));
    }

    TEST(SmallInteger)
    {
        CHECK(Oop::fromInt32(0).isSmallInteger());
        CHECK_EQUAL(0, Oop::fromInt32(0).decodeSmallInteger());
        CHECK_EQUAL(Oop::fromInt32(0), Oop::fromInt32(0));

        CHECK(Oop::fromInt32(-1).isSmallInteger());
        CHECK_EQUAL(-1, Oop::fromInt32(-1).decodeSmallInteger());
        CHECK_EQUAL(Oop::fromInt32(-1), Oop::fromInt32(-1));

        CHECK(Oop::fromInt32(1).isSmallInteger());
        CHECK_EQUAL(1, Oop::fromInt32(1).decodeSmallInteger());
        CHECK_EQUAL(Oop::fromInt32(1), Oop::fromInt32(1));
    }

    TEST(String)
    {
        auto oop = Oop::fromString("TestString");
        CHECK(oop.isPointer());
        CHECK(oop.isNotNil());
        CHECK_EQUAL("TestString", oop.asStdString());
        CHECK_EQUAL("TestString", oop.asString());
        CHECK_EQUAL("'TestString'", oop.printString());
    }

    TEST(Symbol)
    {
        auto oop = Oop::internSymbol("TestSymbol");
        CHECK(oop.isPointer());
        CHECK(oop.isNotNil());
        CHECK_EQUAL("TestSymbol", oop.asStdString());
        CHECK_EQUAL("TestSymbol", oop.asString());
        CHECK_EQUAL("#'TestSymbol'", oop.printString());
        CHECK_EQUAL(oop, Oop::internSymbol("TestSymbol"));
    }

}
