#include "nyast/SmalltalkCompiler//Parser.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace nyast::SmalltalkCompiler;

static ASTNodePtr parseSingleExpression(const std::string &text)
{
    auto node = parseStringWithDoIt(text);
    CHECK(node->isSequenceNode());
    CHECK_EQUAL(1u, node->as<ASTSequenceNode> ().expressions.size());
    return node->as<ASTSequenceNode> ().expressions[0];
}

static ASTNodePtr parseSequence(const std::string &text)
{
    auto node = parseStringWithDoIt(text);
    CHECK(node->isSequenceNode());
    return node;
}

SUITE(Parser)
{
    TEST(Empty)
    {
        auto node = parseStringWithDoIt("");
        CHECK(node->isSequenceNode());
        CHECK(node->as<ASTSequenceNode>().expressions.empty());
    }

    TEST(LiteralInteger)
    {
        auto literal = parseSingleExpression("0");
        CHECK(literal->isLiteralIntegerNode());
        CHECK_EQUAL("0", literal->as<ASTLiteralIntegerNode> ().valueString);

        literal = parseSingleExpression("-0");
        CHECK(literal->isLiteralIntegerNode());
        CHECK_EQUAL("-0", literal->as<ASTLiteralIntegerNode> ().valueString);

        literal = parseSingleExpression("42");
        CHECK(literal->isLiteralIntegerNode());
        CHECK_EQUAL("42", literal->as<ASTLiteralIntegerNode> ().valueString);

        literal = parseSingleExpression("-42");
        CHECK(literal->isLiteralIntegerNode());
        CHECK_EQUAL("-42", literal->as<ASTLiteralIntegerNode> ().valueString);

        literal = parseSingleExpression("(-42)");
        CHECK(literal->isLiteralIntegerNode());
        CHECK_EQUAL("-42", literal->as<ASTLiteralIntegerNode> ().valueString);
    }

    TEST(LiteralFloat)
    {
        auto literal = parseSingleExpression("0.0");
        CHECK(literal->isLiteralFloatNode());
        CHECK_EQUAL(0.0, literal->as<ASTLiteralFloatNode> ().value);

        literal = parseSingleExpression("42.5");
        CHECK(literal->isLiteralFloatNode());
        CHECK_EQUAL(42.5, literal->as<ASTLiteralFloatNode> ().value);

        literal = parseSingleExpression("+42.5e2");
        CHECK(literal->isLiteralFloatNode());
        CHECK_EQUAL(+42.5e2, literal->as<ASTLiteralFloatNode> ().value);

        literal = parseSingleExpression("-1.5e-6");
        CHECK(literal->isLiteralFloatNode());
        CHECK_EQUAL(-1.5e-6, literal->as<ASTLiteralFloatNode> ().value);

        literal = parseSingleExpression("+42.5e+2");
        CHECK(literal->isLiteralFloatNode());
        CHECK_EQUAL(+42.5e+2, literal->as<ASTLiteralFloatNode> ().value);
    }

    TEST(LiteralString)
    {
        auto literal = parseSingleExpression("''");
        CHECK(literal->isLiteralStringNode());
        CHECK_EQUAL("", literal->as<ASTLiteralStringNode> ().value);

        literal = parseSingleExpression("'hello'");
        CHECK(literal->isLiteralStringNode());
        CHECK_EQUAL("hello", literal->as<ASTLiteralStringNode> ().value);

        literal = parseSingleExpression("''''");
        CHECK(literal->isLiteralStringNode());
        CHECK_EQUAL("'", literal->as<ASTLiteralStringNode> ().value);

        literal = parseSingleExpression("'Hello World\r\n'");
        CHECK(literal->isLiteralStringNode());
        CHECK_EQUAL("Hello World\r\n", literal->as<ASTLiteralStringNode> ().value);
    }

    TEST(LiteralCharacter)
    {
        auto literal = parseSingleExpression("$a");
        CHECK(literal->isLiteralCharacterNode());
        CHECK_EQUAL(uint32_t('a'), literal->as<ASTLiteralCharacterNode> ().value);

        literal = parseSingleExpression("$\n");
        CHECK(literal->isLiteralCharacterNode());
        CHECK_EQUAL(uint32_t('\n'), literal->as<ASTLiteralCharacterNode> ().value);

        literal = parseSingleExpression("$\r");
        CHECK(literal->isLiteralCharacterNode());
        CHECK_EQUAL(uint32_t('\r'), literal->as<ASTLiteralCharacterNode> ().value);

        literal = parseSingleExpression("$\t");
        CHECK(literal->isLiteralCharacterNode());
        CHECK_EQUAL(uint32_t('\t'), literal->as<ASTLiteralCharacterNode> ().value);
    }

    TEST(LiteralSymbol)
    {
        auto literal = parseSingleExpression("#a");
        CHECK(literal->isLiteralSymbolNode());
        CHECK_EQUAL("a", literal->as<ASTLiteralSymbolNode> ().value);

        literal = parseSingleExpression("#test:");
        CHECK(literal->isLiteralSymbolNode());
        CHECK_EQUAL("test:", literal->as<ASTLiteralSymbolNode> ().value);

        literal = parseSingleExpression("#<");
        CHECK(literal->isLiteralSymbolNode());
        CHECK_EQUAL("<", literal->as<ASTLiteralSymbolNode> ().value);
    }

    TEST(LiteralSymbolString)
    {
        auto literal = parseSingleExpression("#''");
        CHECK(literal->isLiteralSymbolNode());
        CHECK_EQUAL("", literal->as<ASTLiteralSymbolNode> ().value);

        literal = parseSingleExpression("#'hello'");
        CHECK(literal->isLiteralSymbolNode());
        CHECK_EQUAL("hello", literal->as<ASTLiteralSymbolNode> ().value);

        literal = parseSingleExpression("#'Hello World\r\n'");
        CHECK(literal->isLiteralSymbolNode());
        CHECK_EQUAL("Hello World\r\n", literal->as<ASTLiteralSymbolNode> ().value);

        literal = parseSingleExpression("#''''");
        CHECK(literal->isLiteralSymbolNode());
        CHECK_EQUAL("'", literal->as<ASTLiteralSymbolNode> ().value);

        literal = parseSingleExpression("#'std::string'");
        CHECK(literal->isLiteralSymbolNode());
        CHECK_EQUAL("std::string", literal->as<ASTLiteralSymbolNode> ().value);
    }

    TEST(IdentifierReference)
    {
        auto node = parseSingleExpression("hello");
        CHECK(node->isIdentifierReferenceNode());
        CHECK_EQUAL("hello", node->as<ASTIdentifierReferenceNode> ().identifier);

        node = parseSingleExpression("_helloWorld12345");
        CHECK(node->isIdentifierReferenceNode());
        CHECK_EQUAL("_helloWorld12345", node->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(UnaryMessage)
    {
        auto node = parseSingleExpression("a negated");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("negated", node->as<ASTMessageSendNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(BinaryOperation)
    {
        auto node = parseSingleExpression("a+b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("+", node->as<ASTMessageSendNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(BinaryOperation2)
    {
        auto node = parseSingleExpression("a+b*c");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("*", node->as<ASTMessageSendNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("c", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);

        node = node->as<ASTMessageSendNode> ().receiver;
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("+", node->as<ASTMessageSendNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }


    TEST(KeywordMessage)
    {
        auto node = parseSingleExpression("a computeWith: b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("computeWith:", node->as<ASTMessageSendNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(KeywordMessageNoReceiver)
    {
        auto node = parseSingleExpression("computeWith: b");
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("computeWith:", node->as<ASTMessageSendNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(nullptr, node->as<ASTMessageSendNode> ().receiver);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(UnaryChainMessage)
    {
        auto node = parseSingleExpression("a hello; computeWith: c; yourself");
        CHECK(node->isMessageChainNode());
        CHECK(node->as<ASTMessageChainNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageChainNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK_EQUAL(3u, node->as<ASTMessageChainNode> ().messages.size());

        {
            auto message = node->as<ASTMessageChainNode> ().messages[0];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("hello", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[1];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("c", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[2];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("yourself", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }
    }

    TEST(BinaryChainMessage)
    {
        auto node = parseSingleExpression("a + b; computeWith: c; yourself");
        CHECK(node->isMessageChainNode());
        CHECK(node->as<ASTMessageChainNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageChainNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK_EQUAL(3u, node->as<ASTMessageChainNode> ().messages.size());

        {
            auto message = node->as<ASTMessageChainNode> ().messages[0];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("+", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("b", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[1];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("c", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[2];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("yourself", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }
    }

    TEST(KeywordChainMessage)
    {
        auto node = parseSingleExpression("a computeWith: b; computeWith: c; yourself");
        CHECK(node->isMessageChainNode());
        CHECK(node->as<ASTMessageChainNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageChainNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK_EQUAL(3u, node->as<ASTMessageChainNode> ().messages.size());

        {
            auto message = node->as<ASTMessageChainNode> ().messages[0];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("b", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[1];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("c", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[2];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("yourself", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }
    }

    TEST(KeywordChainMessageNoReceiver)
    {
        auto node = parseSingleExpression("computeWith: b; computeWith: c; yourself");
        CHECK(node->isMessageChainNode());
        CHECK_EQUAL(nullptr, node->as<ASTMessageChainNode> ().receiver);

        CHECK_EQUAL(3u, node->as<ASTMessageChainNode> ().messages.size());

        {
            auto message = node->as<ASTMessageChainNode> ().messages[0];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("b", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[1];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("computeWith:", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments[0]->isIdentifierReferenceNode());
            CHECK_EQUAL("c", message->as<ASTMessageChainMessageNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
        }

        {
            auto message = node->as<ASTMessageChainNode> ().messages[2];
            CHECK(message->isMessageChainMessageNode());
            CHECK_EQUAL("yourself", message->as<ASTMessageChainMessageNode> ().selector->as<ASTLiteralSymbolNode> ().value);

            CHECK(message->as<ASTMessageChainMessageNode> ().arguments.empty());
        }
    }

    TEST(AssignmentOperation)
    {
        auto node = parseSingleExpression("a:=b");
        CHECK(node->isAssignmentNode());
        CHECK(node->as<ASTAssignmentNode> ().variable->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTAssignmentNode> ().variable->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTAssignmentNode> ().value->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTAssignmentNode> ().value->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(AssignmentOperation2)
    {
        auto node = parseSingleExpression("a:=b:=c");
        CHECK(node->isAssignmentNode());
        CHECK(node->as<ASTAssignmentNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL(":=", node->as<ASTAssignmentNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK(node->as<ASTAssignmentNode> ().variable->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTAssignmentNode> ().variable->as<ASTIdentifierReferenceNode> ().identifier);

        node = node->as<ASTAssignmentNode> ().value;
        CHECK(node->isAssignmentNode());
        CHECK(node->as<ASTAssignmentNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL(":=", node->as<ASTAssignmentNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK(node->as<ASTAssignmentNode> ().variable->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTAssignmentNode> ().variable->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTAssignmentNode> ().value->isIdentifierReferenceNode());
        CHECK_EQUAL("c", node->as<ASTAssignmentNode> ().value->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(EmptyArray)
    {
        auto node = parseSingleExpression("{}");
        CHECK(node->isMakeArrayNode());
        CHECK(node->as<ASTMakeArrayNode> ().elements.empty());
    }

    TEST(MakeArray)
    {
        auto node = parseSingleExpression("{a}");
        CHECK(node->isMakeArrayNode());
        auto &elements = node->as<ASTMakeArrayNode> ().elements;
        CHECK_EQUAL(1u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(MakeArray2)
    {
        auto node = parseSingleExpression("{a . b}");
        CHECK(node->isMakeArrayNode());
        auto &elements = node->as<ASTMakeArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);

        element = elements[1];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("b", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(MakeArray3)
    {
        auto node = parseSingleExpression("{a . b . }");
        CHECK(node->isMakeArrayNode());
        auto &elements = node->as<ASTMakeArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);

        element = elements[1];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("b", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(EmptyLiteralArray)
    {
        auto node = parseSingleExpression("#()");
        CHECK(node->isLiteralArrayNode());
        CHECK(node->as<ASTLiteralArrayNode> ().elements.empty());
    }

    TEST(LiteralArray)
    {
        auto node = parseSingleExpression("#(a)");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(1u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(LiteralArray2)
    {
        auto node = parseSingleExpression("#(a b)");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);

        element = elements[1];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("b", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(LiteralArray3)
    {
        auto node = parseSingleExpression("#(hello:World: #b)");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isLiteralSymbolNode());
        CHECK_EQUAL("hello:World:", element->as<ASTLiteralSymbolNode> ().value);

        element = elements[1];
        CHECK(element->isLiteralSymbolNode());
        CHECK_EQUAL("b", element->as<ASTLiteralSymbolNode> ().value);
    }

    TEST(LiteralArray4)
    {
        std::vector operators = {"*", "/", "//", "%", "+", "-", "<<", ">>",
            "<=", ">=", "<", ">", "=", "==", "~=", "~~", "&", "|", "&&",
            "||", "==>", "-->"};
        auto node = parseSingleExpression("#(* / // % + - << >> <= >= < > = == ~= ~~ & | && || ==> --> )");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(operators.size(), elements.size());

        for(size_t i = 0; i < operators.size(); ++i)
        {
            auto element = elements[i];
            CHECK(element->isLiteralSymbolNode());
            CHECK_EQUAL(operators[i], element->as<ASTLiteralSymbolNode> ().value);
        }
    }

    TEST(EmptyLiteralByteArray)
    {
        auto node = parseSingleExpression("#[]");
        CHECK(node->isLiteralByteArrayNode());
        CHECK(node->as<ASTLiteralByteArrayNode> ().elements.empty());
    }

    TEST(LiteralByteArray)
    {
        auto node = parseSingleExpression("#[0]");
        CHECK(node->isLiteralByteArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(1u, elements.size());

        auto element = elements[0];
        CHECK(element->isLiteralIntegerNode());
        CHECK_EQUAL("0", element->as<ASTLiteralIntegerNode> ().valueString);
    }

    TEST(LiteralByteArray2)
    {
        auto node = parseSingleExpression("#[0 1]");
        CHECK(node->isLiteralByteArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isLiteralIntegerNode());
        CHECK_EQUAL("0", element->as<ASTLiteralIntegerNode> ().valueString);

        element = elements[1];
        CHECK(element->isLiteralIntegerNode());
        CHECK_EQUAL("1", element->as<ASTLiteralIntegerNode> ().valueString);
    }

    TEST(SourceWithEmptyLiteralArray)
    {
        auto node = parseStringWithLiteralArrayContent("");
        CHECK(node->isLiteralArrayNode());
        CHECK(node->as<ASTLiteralArrayNode> ().elements.empty());
    }

    TEST(SourceWithLiteralArray)
    {
        auto node = parseStringWithLiteralArrayContent("a");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(1u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(SourceWithLiteralArray2)
    {
        auto node = parseStringWithLiteralArrayContent("a b");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("a", element->as<ASTIdentifierReferenceNode> ().identifier);

        element = elements[1];
        CHECK(element->isIdentifierReferenceNode());
        CHECK_EQUAL("b", element->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(SourceWithLiteralArray3)
    {
        auto node = parseStringWithLiteralArrayContent("hello:World: #b");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(2u, elements.size());

        auto element = elements[0];
        CHECK(element->isLiteralSymbolNode());
        CHECK_EQUAL("hello:World:", element->as<ASTLiteralSymbolNode> ().value);

        element = elements[1];
        CHECK(element->isLiteralSymbolNode());
        CHECK_EQUAL("b", element->as<ASTLiteralSymbolNode> ().value);
    }

    TEST(SourceLiteralArray4)
    {
        std::vector operators = {"*", "/", "//", "%", "+", "-", "<<", ">>",
            "<=", ">=", "<", ">", "=", "==", "~=", "~~", "&", "|", "&&",
            "||", "==>", "-->"};
        auto node = parseStringWithLiteralArrayContent("* / // % + - << >> <= >= < > = == ~= ~~ & | && || ==> -->");
        CHECK(node->isLiteralArrayNode());
        auto &elements = node->as<ASTLiteralArrayNode> ().elements;
        CHECK_EQUAL(operators.size(), elements.size());

        for(size_t i = 0; i < operators.size(); ++i)
        {
            auto element = elements[i];
            CHECK(element->isLiteralSymbolNode());
            CHECK_EQUAL(operators[i], element->as<ASTLiteralSymbolNode> ().value);
        }
    }

    TEST(BlockClosureNode)
    {
        auto node = parseSingleExpression("[]");
        CHECK(node->isBlockClosureNode());
        CHECK(node->as<ASTBlockClosureNode> ().sequence->isSequenceNode());
        CHECK(node->as<ASTBlockClosureNode> ().sequence->as<ASTSequenceNode> ().expressions.empty());
    }

    TEST(BlockClosureNode2)
    {
        auto node = parseSingleExpression("[1]");
        CHECK(node->isBlockClosureNode());
        CHECK(node->as<ASTBlockClosureNode> ().sequence->isSequenceNode());
        CHECK_EQUAL(1u, node->as<ASTBlockClosureNode> ().sequence->as<ASTSequenceNode> ().expressions.size());

        CHECK(node->as<ASTBlockClosureNode> ().sequence->as<ASTSequenceNode> ().expressions[0]->isLiteralIntegerNode());
    }

    TEST(BlockClosureNode3)
    {
        auto node = parseSingleExpression("[| ]");
        CHECK(node->isBlockClosureNode());
        CHECK(node->as<ASTBlockClosureNode> ().arguments.empty());

        CHECK(node->as<ASTBlockClosureNode> ().sequence->isSequenceNode());
        CHECK(node->as<ASTBlockClosureNode> ().sequence->as<ASTSequenceNode> ().expressions.empty());
    }

    TEST(BlockClosureNode4)
    {
        auto node = parseSingleExpression("[:a | ]");
        CHECK(node->isBlockClosureNode());

        auto &blockClosureNode = node->as<ASTBlockClosureNode> ();
        CHECK_EQUAL(1u, blockClosureNode.arguments.size());

        CHECK(blockClosureNode.arguments[0]->isArgumentNode());

        auto argument = &blockClosureNode.arguments[0]->as<ASTArgumentNode> ();
        CHECK_EQUAL("a", argument->identifier);

        CHECK(blockClosureNode.sequence->isSequenceNode());
        CHECK(blockClosureNode.sequence->as<ASTSequenceNode> ().expressions.empty());
    }

    TEST(UnaryPragma)
    {
        auto node = parseSequence("<pragma>");
        CHECK(node->isSequenceNode());
        CHECK_EQUAL(1u, node->as<ASTSequenceNode> ().pragmas.size());

        auto pragma = node->as<ASTSequenceNode> ().pragmas[0];
        CHECK(pragma->isPragmaNode());
        CHECK(pragma->as<ASTPragmaNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("pragma", pragma->as<ASTPragmaNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK(pragma->as<ASTPragmaNode> ().arguments.empty());
    }

    TEST(KeywordPragma)
    {
        auto node = parseSequence("<pragma: 42>");
        CHECK(node->isSequenceNode());
        CHECK_EQUAL(1u, node->as<ASTSequenceNode> ().pragmas.size());

        auto pragma = node->as<ASTSequenceNode> ().pragmas[0];
        CHECK(pragma->isPragmaNode());
        CHECK(pragma->as<ASTPragmaNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("pragma:", pragma->as<ASTPragmaNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(1u, pragma->as<ASTPragmaNode> ().arguments.size());
        CHECK(pragma->as<ASTPragmaNode> ().arguments[0]->isLiteralIntegerNode());
        CHECK_EQUAL("42", pragma->as<ASTPragmaNode> ().arguments[0]->as<ASTLiteralIntegerNode> ().valueString);
    }

    TEST(KeywordPragma2)
    {
        auto node = parseSequence("<pragma: 42 value: -5>");
        CHECK(node->isSequenceNode());
        CHECK_EQUAL(1u, node->as<ASTSequenceNode> ().pragmas.size());

        auto pragma = node->as<ASTSequenceNode> ().pragmas[0];
        CHECK(pragma->isPragmaNode());
        CHECK(pragma->as<ASTPragmaNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("pragma:value:", pragma->as<ASTPragmaNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(2u, pragma->as<ASTPragmaNode> ().arguments.size());

        CHECK(pragma->as<ASTPragmaNode> ().arguments[0]->isLiteralIntegerNode());
        CHECK_EQUAL("42", pragma->as<ASTPragmaNode> ().arguments[0]->as<ASTLiteralIntegerNode> ().valueString);

        CHECK(pragma->as<ASTPragmaNode> ().arguments[1]->isLiteralIntegerNode());
        CHECK_EQUAL("-5", pragma->as<ASTPragmaNode> ().arguments[1]->as<ASTLiteralIntegerNode> ().valueString);
    }

    TEST(ReturnNode)
    {
        auto node = parseSingleExpression("^ 42");
        CHECK(node->isReturnNode());
        CHECK(node->as<ASTReturnNode> ().value->isLiteralIntegerNode());
        CHECK_EQUAL("42", node->as<ASTReturnNode> ().value->as<ASTLiteralIntegerNode> ().valueString);
    }

    TEST(UnaryMethod)
    {
        auto node = parseStringWithMethod("negated");
        CHECK(node->isMethodNode());
        CHECK(node->as<ASTMethodNode>().selector->isLiteralSymbolNode());
        CHECK(node->as<ASTMethodNode>().arguments.empty());
        CHECK_EQUAL("negated", node->as<ASTMethodNode>().selector->as<ASTLiteralSymbolNode> ().value);
    }

    TEST(UnaryMethod2)
    {
        auto node = parseStringWithMethod("negated ^ 42");
        CHECK(node->isMethodNode());
        CHECK(node->as<ASTMethodNode>().selector->isLiteralSymbolNode());
        CHECK(node->as<ASTMethodNode>().arguments.empty());
        CHECK_EQUAL("negated", node->as<ASTMethodNode>().selector->as<ASTLiteralSymbolNode> ().value);

        node = node->as<ASTMethodNode> ().sequence;
        CHECK(node->isSequenceNode());
        CHECK_EQUAL(1u, node->as<ASTSequenceNode> ().expressions.size());

        node = node->as<ASTSequenceNode> ().expressions[0];
        CHECK(node->isReturnNode());
        CHECK(node->as<ASTReturnNode> ().value->isLiteralIntegerNode());
        CHECK_EQUAL("42", node->as<ASTReturnNode> ().value->as<ASTLiteralIntegerNode> ().valueString);
    }

    TEST(BinaryMethod)
    {
        auto node = parseStringWithMethod("+ other");
        CHECK(node->isMethodNode());
        CHECK(node->as<ASTMethodNode>().selector->isLiteralSymbolNode());
        CHECK_EQUAL("+", node->as<ASTMethodNode>().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(1u, node->as<ASTMethodNode>().arguments.size());
        CHECK_EQUAL("other", node->as<ASTMethodNode>().arguments[0]->as<ASTArgumentNode> ().identifier);
    }

    TEST(BinaryMethod2)
    {
        auto node = parseStringWithMethod("^ other ^ other");
        CHECK(node->isMethodNode());
        CHECK(node->as<ASTMethodNode>().selector->isLiteralSymbolNode());
        CHECK_EQUAL("^", node->as<ASTMethodNode>().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(1u, node->as<ASTMethodNode>().arguments.size());
        CHECK_EQUAL("other", node->as<ASTMethodNode>().arguments[0]->as<ASTArgumentNode> ().identifier);

        node = node->as<ASTMethodNode> ().sequence;
        CHECK(node->isSequenceNode());
        CHECK_EQUAL(1u, node->as<ASTSequenceNode> ().expressions.size());

        node = node->as<ASTSequenceNode> ().expressions[0];
        CHECK(node->isReturnNode());
        CHECK(node->as<ASTReturnNode> ().value->isIdentifierReferenceNode());
        CHECK_EQUAL("other", node->as<ASTReturnNode> ().value->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(KeywordMethod)
    {
        auto node = parseStringWithMethod("return: other");
        CHECK(node->isMethodNode());
        CHECK(node->as<ASTMethodNode>().selector->isLiteralSymbolNode());
        CHECK_EQUAL("return:", node->as<ASTMethodNode>().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(1u, node->as<ASTMethodNode>().arguments.size());
        CHECK_EQUAL("other", node->as<ASTMethodNode>().arguments[0]->as<ASTArgumentNode> ().identifier);
    }

    TEST(KeywordMethod2)
    {
        auto node = parseStringWithMethod("return: other ^ other");
        CHECK(node->isMethodNode());
        CHECK(node->as<ASTMethodNode>().selector->isLiteralSymbolNode());
        CHECK_EQUAL("return:", node->as<ASTMethodNode>().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(1u, node->as<ASTMethodNode>().arguments.size());
        CHECK_EQUAL("other", node->as<ASTMethodNode>().arguments[0]->as<ASTArgumentNode> ().identifier);

        node = node->as<ASTMethodNode> ().sequence;
        CHECK(node->isSequenceNode());
        CHECK_EQUAL(1u, node->as<ASTSequenceNode> ().expressions.size());

        node = node->as<ASTSequenceNode> ().expressions[0];
        CHECK(node->isReturnNode());
        CHECK(node->as<ASTReturnNode> ().value->isIdentifierReferenceNode());
        CHECK_EQUAL("other", node->as<ASTReturnNode> ().value->as<ASTIdentifierReferenceNode> ().identifier);
    }

    TEST(KeywordMethod3)
    {
        auto node = parseStringWithMethod("sum: a with: b ^ a + b");
        CHECK(node->isMethodNode());
        CHECK(node->as<ASTMethodNode>().selector->isLiteralSymbolNode());
        CHECK_EQUAL("sum:with:", node->as<ASTMethodNode>().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(2u, node->as<ASTMethodNode>().arguments.size());
        CHECK_EQUAL("a", node->as<ASTMethodNode>().arguments[0]->as<ASTArgumentNode> ().identifier);
        CHECK_EQUAL("b", node->as<ASTMethodNode>().arguments[1]->as<ASTArgumentNode> ().identifier);

        node = node->as<ASTMethodNode> ().sequence;
        CHECK(node->isSequenceNode());
        CHECK_EQUAL(1u, node->as<ASTSequenceNode> ().expressions.size());

        node = node->as<ASTSequenceNode> ().expressions[0];
        CHECK(node->isReturnNode());

        node = node->as<ASTReturnNode> ().value;
        CHECK(node->isMessageSendNode());
        CHECK(node->as<ASTMessageSendNode> ().selector->isLiteralSymbolNode());
        CHECK_EQUAL("+", node->as<ASTMessageSendNode> ().selector->as<ASTLiteralSymbolNode> ().value);

        CHECK_EQUAL(1u, node->as<ASTMessageSendNode> ().arguments.size());

        CHECK(node->as<ASTMessageSendNode> ().receiver->isIdentifierReferenceNode());
        CHECK_EQUAL("a", node->as<ASTMessageSendNode> ().receiver->as<ASTIdentifierReferenceNode> ().identifier);

        CHECK(node->as<ASTMessageSendNode> ().arguments[0]->isIdentifierReferenceNode());
        CHECK_EQUAL("b", node->as<ASTMessageSendNode> ().arguments[0]->as<ASTIdentifierReferenceNode> ().identifier);
    }
}
