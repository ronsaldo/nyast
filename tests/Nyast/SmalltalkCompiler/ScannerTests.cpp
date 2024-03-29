#include "nyast/SmalltalkCompiler/Scanner.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace nyast::SmalltalkCompiler;

static TokenType scanSingleTokenType(const std::string &source)
{
    auto tokenListPtr = scanString(source);
    auto &tokenList = *tokenListPtr;
    CHECK(tokenList.size() == 1 || (tokenList.size() == 2 && tokenList[1].type == TokenType::EndOfSource));
    return tokenList[0].type;
}

static std::string scanSingleTokenText(const std::string &source)
{
    auto tokenListPtr = scanString(source);
    auto &tokenList = *tokenListPtr;
    CHECK(tokenList.size() == 1 || (tokenList.size() == 2 && tokenList[1].type == TokenType::EndOfSource));
    return tokenList[0].text();
}

SUITE(SmalltalkScanner)
{
    TEST(EndOfSource)
    {
        CHECK_EQUAL(TokenType::EndOfSource, scanSingleTokenType(""));
        CHECK_EQUAL(TokenType::EndOfSource, scanSingleTokenType("\" \""));
        CHECK_EQUAL(TokenType::ErrorIncompleteComment, scanSingleTokenType("\""));
    }

    TEST(Identifiers)
    {
        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("A"));
        CHECK_EQUAL("A", scanSingleTokenText("A"));

        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("_"));
        CHECK_EQUAL("_", scanSingleTokenText("_"));

        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("a"));
        CHECK_EQUAL("a", scanSingleTokenText("a"));

        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("hello"));
        CHECK_EQUAL("hello", scanSingleTokenText("hello"));

        CHECK_EQUAL(TokenType::Identifier, scanSingleTokenType("helloWorld01234"));
        CHECK_EQUAL("helloWorld01234", scanSingleTokenText("helloWorld01234"));
    }

    TEST(Keywords)
    {
        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("A:"));
        CHECK_EQUAL("A:", scanSingleTokenText("A:"));

        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("_:"));
        CHECK_EQUAL("_:", scanSingleTokenText("_:"));

        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("a:"));
        CHECK_EQUAL("a:", scanSingleTokenText("a:"));

        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("helloWorld01234:"));
        CHECK_EQUAL("helloWorld01234:", scanSingleTokenText("helloWorld01234:"));

        CHECK_EQUAL(TokenType::Keyword, scanSingleTokenType("helloWorld01234:A:a1:_:"));
        CHECK_EQUAL("helloWorld01234:A:a1:_:", scanSingleTokenText("helloWorld01234:A:a1:_:"));
    }

    TEST(SymbolIdentifiers)
    {
        CHECK_EQUAL(TokenType::SymbolIdentifier, scanSingleTokenType("#A"));
        CHECK_EQUAL("#A", scanSingleTokenText("#A"));

        CHECK_EQUAL(TokenType::SymbolIdentifier, scanSingleTokenType("#_"));
        CHECK_EQUAL("#_", scanSingleTokenText("#_"));

        CHECK_EQUAL(TokenType::SymbolIdentifier, scanSingleTokenType("#a"));
        CHECK_EQUAL("#a", scanSingleTokenText("#a"));

        CHECK_EQUAL(TokenType::SymbolIdentifier, scanSingleTokenType("#helloWorld01234"));
        CHECK_EQUAL("#helloWorld01234", scanSingleTokenText("#helloWorld01234"));
    }

    TEST(SymbolKeyword)
    {
        CHECK_EQUAL(TokenType::SymbolKeyword, scanSingleTokenType("#A:"));
        CHECK_EQUAL("#A:", scanSingleTokenText("#A:"));

        CHECK_EQUAL(TokenType::SymbolKeyword, scanSingleTokenType("#_:"));
        CHECK_EQUAL("#_:", scanSingleTokenText("#_:"));

        CHECK_EQUAL(TokenType::SymbolKeyword, scanSingleTokenType("#a:"));
        CHECK_EQUAL("#a:", scanSingleTokenText("#a:"));

        CHECK_EQUAL(TokenType::SymbolKeyword, scanSingleTokenType("#helloWorld01234:"));
        CHECK_EQUAL("#helloWorld01234:", scanSingleTokenText("#helloWorld01234:"));

        CHECK_EQUAL(TokenType::SymbolKeyword, scanSingleTokenType("#helloWorld01234:A:a1:_:"));
        CHECK_EQUAL("#helloWorld01234:A:a1:_:", scanSingleTokenText("#helloWorld01234:A:a1:_:"));
    }

    TEST(SymbolOperator)
    {
        CHECK_EQUAL(TokenType::SymbolOperator, scanSingleTokenType("#<"));
        CHECK_EQUAL("#<", scanSingleTokenText("#<"));

        CHECK_EQUAL(TokenType::SymbolOperator, scanSingleTokenType("#=="));
        CHECK_EQUAL("#==", scanSingleTokenText("#=="));

        CHECK_EQUAL(TokenType::SymbolOperator, scanSingleTokenType("#->"));
        CHECK_EQUAL("#->", scanSingleTokenText("#->"));

        CHECK_EQUAL(TokenType::SymbolOperator, scanSingleTokenType("#=>"));
        CHECK_EQUAL("#=>", scanSingleTokenText("#=>"));
    }

    TEST(Integers)
    {
        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("0"));
        CHECK_EQUAL("0", scanSingleTokenText("0"));

        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("0123456789"));
        CHECK_EQUAL("0123456789", scanSingleTokenText("0123456789"));

        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("-0123456789"));
        CHECK_EQUAL("-0123456789", scanSingleTokenText("-0123456789"));

        CHECK_EQUAL(TokenType::Integer, scanSingleTokenType("32rHELLO012364WORLD"));
        CHECK_EQUAL("32rHELLO012364WORLD", scanSingleTokenText("32rHELLO012364WORLD"));
    }

    TEST(Floats)
    {
        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("0.0"));
        CHECK_EQUAL("0.0", scanSingleTokenText("0.0"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("-0.0"));
        CHECK_EQUAL("-0.0", scanSingleTokenText("-0.0"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("-0e-53"));
        CHECK_EQUAL("-0e-53", scanSingleTokenText("-0e-53"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("0e53"));
        CHECK_EQUAL("0e53", scanSingleTokenText("0e53"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("0123456789.14565"));
        CHECK_EQUAL("0123456789.14565", scanSingleTokenText("0123456789.14565"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("-0123456789.14565"));
        CHECK_EQUAL("-0123456789.14565", scanSingleTokenText("-0123456789.14565"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("0123456789.14565e53"));
        CHECK_EQUAL("0123456789.14565e53", scanSingleTokenText("0123456789.14565e53"));

        CHECK_EQUAL(TokenType::Float, scanSingleTokenType("-0123456789.14565e-53"));
        CHECK_EQUAL("-0123456789.14565e-53", scanSingleTokenText("-0123456789.14565e-53"));
    }

    TEST(Character)
    {
        CHECK_EQUAL(TokenType::Character, scanSingleTokenType("$a"));
        CHECK_EQUAL("$a", scanSingleTokenText("$a"));

        CHECK_EQUAL(TokenType::Character, scanSingleTokenType("$$"));
        CHECK_EQUAL("$$", scanSingleTokenText("$$"));
    }

    TEST(String)
    {
        CHECK_EQUAL(TokenType::String, scanSingleTokenType("''"));
        CHECK_EQUAL("''", scanSingleTokenText("''"));

        CHECK_EQUAL(TokenType::String, scanSingleTokenType("'a'"));
        CHECK_EQUAL("'a'", scanSingleTokenText("'a'"));

        CHECK_EQUAL(TokenType::String, scanSingleTokenType("''''"));
        CHECK_EQUAL("''''", scanSingleTokenText("''''"));

        CHECK_EQUAL(TokenType::ErrorIncompleteString, scanSingleTokenType("'"));
        CHECK_EQUAL("'", scanSingleTokenText("'"));

        CHECK_EQUAL(TokenType::ErrorIncompleteString, scanSingleTokenType("'''"));
        CHECK_EQUAL("'''", scanSingleTokenText("'''"));
    }

    TEST(SymbolString)
    {
        CHECK_EQUAL(TokenType::SymbolString, scanSingleTokenType("#''"));
        CHECK_EQUAL("#''", scanSingleTokenText("#''"));

        CHECK_EQUAL(TokenType::SymbolString, scanSingleTokenType("#'a'"));
        CHECK_EQUAL("#'a'", scanSingleTokenText("#'a'"));

        CHECK_EQUAL(TokenType::SymbolString, scanSingleTokenType("#''''"));
        CHECK_EQUAL("#''''", scanSingleTokenText("#''''"));

        CHECK_EQUAL(TokenType::ErrorIncompleteSymbolString, scanSingleTokenType("#'"));
        CHECK_EQUAL("#'", scanSingleTokenText("#'"));

        CHECK_EQUAL(TokenType::ErrorIncompleteSymbolString, scanSingleTokenType("#'''"));
        CHECK_EQUAL("#'''", scanSingleTokenText("#'''"));
    }

    TEST(SpecialOperators)
    {
        CHECK_EQUAL(TokenType::Bar, scanSingleTokenType("|"));
        CHECK_EQUAL(TokenType::Caret, scanSingleTokenType("^"));
        CHECK_EQUAL(TokenType::LessThan, scanSingleTokenType("<"));
        CHECK_EQUAL(TokenType::GreaterThan, scanSingleTokenType(">"));
        CHECK_EQUAL(TokenType::Colon, scanSingleTokenType(":"));
        CHECK_EQUAL(TokenType::Assignment, scanSingleTokenType(":="));
        CHECK_EQUAL(TokenType::ColonColon, scanSingleTokenType("::"));
    }

    TEST(Delimiters)
    {
        CHECK_EQUAL(TokenType::Dot, scanSingleTokenType("."));
        CHECK_EQUAL(TokenType::Semicolon, scanSingleTokenType(";"));
        CHECK_EQUAL(TokenType::LeftParent, scanSingleTokenType("("));
        CHECK_EQUAL(TokenType::RightParent, scanSingleTokenType(")"));
        CHECK_EQUAL(TokenType::LeftBracket, scanSingleTokenType("["));
        CHECK_EQUAL(TokenType::RightBracket, scanSingleTokenType("]"));
        CHECK_EQUAL(TokenType::LeftCurlyBracket, scanSingleTokenType("{"));
        CHECK_EQUAL(TokenType::RightCurlyBracket, scanSingleTokenType("}"));

        CHECK_EQUAL(TokenType::LiteralArrayLeftParent, scanSingleTokenType("#("));
        CHECK_EQUAL(TokenType::ByteArrayLeftBracket, scanSingleTokenType("#["));
    }

    TEST(Operators)
    {
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("=>"));

        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("||"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("&&"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("&"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("="));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("=="));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("~="));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("~~"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("<="));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType(">="));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("<<"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType(">>"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("+"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("-"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("*"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("/"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("//"));
        CHECK_EQUAL(TokenType::BinaryOperator, scanSingleTokenType("%"));
    }
}
