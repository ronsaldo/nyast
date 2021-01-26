#include "nyast/SmalltalkCompiler/Parser.hpp"
#include "nyast/SmalltalkCompiler/Visitors.hpp"
#include "nyast/SmalltalkCompiler/Evaluator.hpp"
#include <sstream>

namespace nyast
{
namespace SmalltalkCompiler
{

struct ASTEvaluator : ASTVisitor
{
    virtual std::any visitParseErrorNode(ASTParseErrorNode &node) override
    {
        throw std::runtime_error(node.errorMessage);
    }

    virtual std::any visitSequenceNode(ASTSequenceNode &node) override
    {
        auto result = Oop::nil();
        for(auto &expression : node.expressions)
            result = std::any_cast<Oop> (visitNode(*expression));
        return result;
    }

    virtual std::any visitMethodNode(ASTMethodNode &) override { abort(); }
    virtual std::any visitLocalDeclarationsNode(ASTLocalDeclarationsNode &) override { abort(); }
    virtual std::any visitLocalDeclarationNode(ASTLocalDeclarationNode &) override { abort(); }
    virtual std::any visitPragmaNode(ASTPragmaNode &) override { abort(); }
    virtual std::any visitBlockClosureNode(ASTBlockClosureNode &) override { abort(); }
    virtual std::any visitArgumentNode(ASTArgumentNode &) override { abort(); }

    virtual std::any visitLiteralIntegerNode(ASTLiteralIntegerNode &node) override
    {
        // FIXME: Support large integers and perform proper parsing of these literals!!!
        return Oop::fromInt64(atoll(node.valueString.c_str()));
    }

    virtual std::any visitLiteralFloatNode(ASTLiteralFloatNode &node) override
    {
        return Oop::fromFloat64(node.value);
    }

    virtual std::any visitLiteralCharacterNode(ASTLiteralCharacterNode &node) override
    {
        return Oop::fromChar32(node.value);
    }

    virtual std::any visitLiteralStringNode(ASTLiteralStringNode &node) override
    {
        return Oop::fromString(node.value);
    }

    virtual std::any visitLiteralSymbolNode(ASTLiteralSymbolNode &node) override
    {
        return Oop::internSymbol(node.value);
    }

    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &node) override
    {
        OopList array;
        array.reserve(node.elements.size());
        for(auto &element : node.elements)
            array.push_back(std::any_cast<Oop> (visitNode(*element)));
        return Oop::fromOopList(array);
    }

    virtual std::any visitLiteralByteArrayNode(ASTLiteralByteArrayNode &node) override
    {
        ByteArrayData byteArrayData;
        byteArrayData.reserve(node.elements.size());
        for(auto &element : node.elements)
        {
            auto value = std::any_cast<Oop> (visitNode(*element));
            if(!value.isSmallInteger() || value.decodeSmallInteger() < 0 || value.decodeSmallInteger() > 255)
                throw std::runtime_error("Byte array constant must be between 0 and 255 instead of " + value.asString() + ".");
            byteArrayData.push_back(uint8_t(value.decodeSmallInteger()));
        }

        return Oop::fromByteArray(byteArrayData);
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        if(node.identifier == "nil")
            return Oop::nil();
        else if(node.identifier == "true")
            return Oop::trueValue();
        else if(node.identifier == "false")
            return Oop::falseValue();
        throw std::runtime_error("Failed to solve identifier reference " + node.identifier);
    }

    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) override
    {
        auto receiver = std::any_cast<Oop> (visitNode(*node.receiver));
        auto selector = std::any_cast<Oop> (visitNode(*node.selector));
        OopList arguments;
        arguments.reserve(node.arguments.size());
        for(auto &argNode : node.arguments)
            arguments.push_back(std::any_cast<Oop> (visitNode(*argNode)));

        return receiver.performWithArguments(selector, arguments);
    }

    virtual std::any visitMessageChainNode(ASTMessageChainNode &) override { abort(); }
    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &) override { abort(); }
    virtual std::any visitAssignmentNode(ASTAssignmentNode &) override { abort(); }
    virtual std::any visitReturnNode(ASTReturnNode &) override { abort(); }

    virtual std::any visitMakeArrayNode(ASTMakeArrayNode &node) override
    {
        OopList array;
        array.reserve(node.elements.size());
        for(auto &element : node.elements)
            array.push_back(std::any_cast<Oop> (visitNode(*element)));
        return Oop::fromOopList(array);
    }
};

Oop evaluateValidatedParsedDoIt(ASTNodePtr ast)
{
    return std::any_cast<Oop> (ASTEvaluator().visitNode(*ast));
}

Oop evaluateDoItString(const std::string &string, const std::string &fileName)
{
    std::ostringstream parseErrors;
    auto ast = SmalltalkCompiler::parseStringWithDoIt(string, fileName);
    if(!SmalltalkCompiler::validateASTParseErrors(ast, [&](SmalltalkCompiler::ASTParseErrorNode &parseErrorNode) {
        parseErrors << parseErrorNode.sourcePosition << ": " << parseErrorNode.errorMessage << '\n';
        parseErrors << parseErrorNode.sourcePosition.content() << '\n';
    }))
    {
        throw std::runtime_error(parseErrors.str());
    }

    return evaluateValidatedParsedDoIt(ast);
}


} // End of namespace SmalltalkCompiler
} // End of namespace nyast
