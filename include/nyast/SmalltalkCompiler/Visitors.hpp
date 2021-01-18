#ifndef NYAST_SMALLTAK_COMPILER_VISITORS_HPP
#define NYAST_SMALLTAK_COMPILER_VISITORS_HPP
#pragma once

#include "AST.hpp"

namespace nyast
{
namespace SmalltalkCompiler
{

struct ASTSequentialVisitor : ASTVisitor
{
    void visitNodeList(ASTNodePtrList &nodeList)
    {
        for(auto &node : nodeList)
            visitNode(*node);
    }

    virtual std::any visitParseErrorNode(ASTParseErrorNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitSequenceNode(ASTSequenceNode &node) override
    {
        visitNodeList(node.expressions);
        return std::any();
    }

    virtual std::any visitMethodNode(ASTMethodNode &node) override
    {
        visitNode(*node.selector);
        visitNodeList(node.arguments);
        visitNode(*node.sequence);
        return std::any();
    }

    virtual std::any visitPragmaNode(ASTPragmaNode &node) override
    {
        visitNode(*node.selector);
        visitNodeList(node.arguments);
        return std::any();
    }


    virtual std::any visitLocalDeclarationsNode(ASTLocalDeclarationsNode &node) override
    {
        visitNodeList(node.declarations);
        return std::any();
    }

    virtual std::any visitLocalDeclarationNode(ASTLocalDeclarationNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitBlockClosureNode(ASTBlockClosureNode &node) override
    {
        visitNodeList(node.arguments);
        visitNode(*node.sequence);
        return std::any();
    }

    virtual std::any visitArgumentNode(ASTArgumentNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitLiteralIntegerNode(ASTLiteralIntegerNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitLiteralFloatNode(ASTLiteralFloatNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitLiteralCharacterNode(ASTLiteralCharacterNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitLiteralStringNode(ASTLiteralStringNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitLiteralSymbolNode(ASTLiteralSymbolNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &node) override
    {
        visitNodeList(node.elements);
        return std::any();
    }

    virtual std::any visitLiteralByteArrayNode(ASTLiteralByteArrayNode &node) override
    {
        visitNodeList(node.elements);
        return std::any();
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) override
    {
        visitNode(*node.selector);
        visitNode(*node.receiver);
        visitNodeList(node.arguments);
        return std::any();
    }

    virtual std::any visitMessageChainNode(ASTMessageChainNode &node) override
    {
        visitNode(*node.receiver);
        visitNodeList(node.messages);
        return std::any();
    }

    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &node) override
    {
        visitNode(*node.selector);
        visitNodeList(node.arguments);
        return std::any();
    }

    virtual std::any visitMakeArrayNode(ASTMakeArrayNode &node) override
    {
        visitNodeList(node.elements);
        return std::any();
    }

    virtual std::any visitAssignmentNode(ASTAssignmentNode &node) override
    {
        visitNode(*node.variable);
        visitNode(*node.selector);
        visitNode(*node.value);
        return std::any();
    }

    virtual std::any visitReturnNode(ASTReturnNode &node) override
    {
        visitNode(*node.value);
        return std::any();
    }
};

template<typename FT>
bool validateASTParseErrors(const ASTNodePtr &ast, const FT &parseErrorCallback)
{
    struct ASTParseErrorValidatingVisitor : ASTSequentialVisitor
    {
        ASTParseErrorValidatingVisitor(const FT &cparseErrorCallback)
            : parseErrorCallback(cparseErrorCallback) {}

        const FT &parseErrorCallback;
        bool hasParseError = false;

        virtual std::any visitParseErrorNode(ASTParseErrorNode &node) override
        {
            hasParseError = true;
            parseErrorCallback(node);
            return std::any();
        }
    };

    ASTParseErrorValidatingVisitor visitor(parseErrorCallback);
    visitor.visitNode(*ast);
    return !visitor.hasParseError;
}

} // End of namespace SmalltalkCompiler
} // End of namespace nyast

#endif //NYAST_SMALLTAK_COMPILER_AST_HPP
