#ifndef NYAST_SMALLTAK_COMPILER_AST_HPP
#define NYAST_SMALLTAK_COMPILER_AST_HPP
#pragma once

#include "Token.hpp"
#include <memory>
#include <any>

namespace nyast
{
namespace SmalltalkCompiler
{

struct ASTNode;
struct ASTParseErrorNode;
struct ASTMethodNode;
struct ASTArgumentNode;
struct ASTBlockClosureNode;
struct ASTPragmaNode;
struct ASTLocalDeclarationNode;
struct ASTLocalDeclarationsNode;
struct ASTSequenceNode;

struct ASTLiteralIntegerNode;
struct ASTLiteralFloatNode;
struct ASTLiteralCharacterNode;
struct ASTLiteralStringNode;
struct ASTLiteralSymbolNode;
struct ASTLiteralArrayNode;
struct ASTLiteralByteArrayNode;

struct ASTIdentifierReferenceNode;
struct ASTMessageSendNode;
struct ASTMessageChainNode;
struct ASTMessageChainMessageNode;
struct ASTAssignmentNode;
struct ASTReturnNode;

struct ASTMakeArrayNode;

struct ASTVisitor;
struct ASTNode : std::enable_shared_from_this<ASTNode>
{
    virtual ~ASTNode() {}
    virtual std::any accept(ASTVisitor &visitor) = 0;

    virtual bool isParseErrorNode() const { return false; }
    virtual bool isMethodNode() const { return false; }
    virtual bool isSequenceNode() const { return false; }
    virtual bool isLocalDeclarationsNode() const { return false; }
    virtual bool isLocalDeclarationNode() const { return false; }
    virtual bool isPragmaNode() const { return false; }
    virtual bool isBlockClosureNode() const { return false; }
    virtual bool isArgumentNode() const { return false; }

    virtual bool isLiteralIntegerNode() const { return false; }
    virtual bool isLiteralFloatNode() const { return false; }
    virtual bool isLiteralCharacterNode() const { return false; }
    virtual bool isLiteralStringNode() const { return false; }
    virtual bool isLiteralSymbolNode() const { return false; }
    virtual bool isLiteralArrayNode() const { return false; }
    virtual bool isLiteralByteArrayNode() const { return false; }

    virtual bool isIdentifierReferenceNode() const { return false; }
    virtual bool isMessageSendNode() const { return false; }
    virtual bool isMessageChainNode() const { return false; }
    virtual bool isMessageChainMessageNode() const { return false; }
    virtual bool isAssignmentNode() const { return false; }
    virtual bool isReturnNode() const { return false; }

    virtual bool isMakeArrayNode() const { return false; }

    template<typename T>
    T &as()
    {
        return static_cast<T&> (*this);
    }

    template<typename T>
    const T &as() const
    {
        return static_cast<const T&> (*this);
    }

    void setTokenRange(const TokenRange &tokenRange)
    {
        tokens = tokenRange;
        sourcePosition = tokenRange.asSourcePosition();
    }

    TokenRange tokens;
    SourcePosition sourcePosition;
};

typedef std::shared_ptr<ASTNode> ASTNodePtr;
typedef std::vector<ASTNodePtr> ASTNodePtrList;

struct ASTVisitor
{
    virtual std::any visitNode(ASTNode &node)
    {
        return node.accept(*this);
    }

    virtual std::any visitParseErrorNode(ASTParseErrorNode &node) = 0;
    virtual std::any visitSequenceNode(ASTSequenceNode &node) = 0;
    virtual std::any visitMethodNode(ASTMethodNode &node) = 0;
    virtual std::any visitLocalDeclarationsNode(ASTLocalDeclarationsNode &node) = 0;
    virtual std::any visitLocalDeclarationNode(ASTLocalDeclarationNode &node) = 0;
    virtual std::any visitPragmaNode(ASTPragmaNode &node) = 0;
    virtual std::any visitBlockClosureNode(ASTBlockClosureNode &node) = 0;
    virtual std::any visitArgumentNode(ASTArgumentNode &node) = 0;

    virtual std::any visitLiteralIntegerNode(ASTLiteralIntegerNode &node) = 0;
    virtual std::any visitLiteralFloatNode(ASTLiteralFloatNode &node) = 0;
    virtual std::any visitLiteralCharacterNode(ASTLiteralCharacterNode &node) = 0;
    virtual std::any visitLiteralStringNode(ASTLiteralStringNode &node) = 0;
    virtual std::any visitLiteralSymbolNode(ASTLiteralSymbolNode &node) = 0;
    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &node) = 0;
    virtual std::any visitLiteralByteArrayNode(ASTLiteralByteArrayNode &node) = 0;

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) = 0;
    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) = 0;
    virtual std::any visitMessageChainNode(ASTMessageChainNode &node) = 0;
    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &node) = 0;
    virtual std::any visitAssignmentNode(ASTAssignmentNode &node) = 0;
    virtual std::any visitReturnNode(ASTReturnNode &node) = 0;
    virtual std::any visitMakeArrayNode(ASTMakeArrayNode &node) = 0;
};

struct ASTMethodNode : ASTNode
{
    ASTNodePtr selector;
    ASTNodePtrList arguments;
    ASTNodePtr sequence;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMethodNode(*this);
    }

    virtual bool isMethodNode() const override
    {
        return true;
    }
};

struct ASTSequenceNode : ASTNode
{
    ASTNodePtrList pragmas;
    ASTNodePtr localDeclarations;
    ASTNodePtrList expressions;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitSequenceNode(*this);
    }

    virtual bool isSequenceNode() const override
    {
        return true;
    }
};

struct ASTPragmaNode : ASTNode
{
    ASTNodePtr selector;
    ASTNodePtrList arguments;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitPragmaNode(*this);
    }

    virtual bool isPragmaNode() const override
    {
        return true;
    }
};

struct ASTArgumentNode : ASTNode
{
    std::string identifier;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitArgumentNode(*this);
    }

    virtual bool isArgumentNode() const override
    {
        return true;
    }
};

struct ASTLocalDeclarationNode : ASTNode
{
    std::string identifier;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLocalDeclarationNode(*this);
    }

    virtual bool isLocalDeclarationNode() const override
    {
        return true;
    }
};

struct ASTLocalDeclarationsNode : ASTNode
{
    ASTNodePtrList declarations;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLocalDeclarationsNode(*this);
    }

    virtual bool isLocalDeclarationsNode() const override
    {
        return true;
    }
};

struct ASTBlockClosureNode : ASTNode
{
    ASTNodePtrList arguments;
    ASTNodePtr sequence;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitBlockClosureNode(*this);
    }

    virtual bool isBlockClosureNode() const override
    {
        return true;
    }

};

struct ASTLiteralIntegerNode : ASTNode
{
    std::string valueString;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLiteralIntegerNode(*this);
    }

    virtual bool isLiteralIntegerNode() const override
    {
        return true;
    }
};

struct ASTLiteralFloatNode : ASTNode
{
    double value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLiteralFloatNode(*this);
    }

    virtual bool isLiteralFloatNode() const override
    {
        return true;
    }
};

struct ASTLiteralCharacterNode : ASTNode
{
    uint32_t value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLiteralCharacterNode(*this);
    }

    virtual bool isLiteralCharacterNode() const override
    {
        return true;
    }
};

struct ASTLiteralStringNode : ASTNode
{
    std::string value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLiteralStringNode(*this);
    }

    virtual bool isLiteralStringNode() const override
    {
        return true;
    }
};

struct ASTLiteralSymbolNode : ASTNode
{
    std::string value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLiteralSymbolNode(*this);
    }

    virtual bool isLiteralSymbolNode() const override
    {
        return true;
    }
};

struct ASTParseErrorNode : ASTNode
{
    std::string errorMessage;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitParseErrorNode(*this);
    }

    virtual bool isParseErrorNode() const override
    {
        return true;
    }
};

struct ASTIdentifierReferenceNode : ASTNode
{
    std::string identifier;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitIdentifierReferenceNode(*this);
    }

    virtual bool isIdentifierReferenceNode() const override
    {
        return true;
    }
};

struct ASTMessageSendNode : ASTNode
{
    ASTNodePtr selector;
    ASTNodePtr receiver;
    ASTNodePtrList arguments;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMessageSendNode(*this);
    }

    virtual bool isMessageSendNode() const override
    {
        return true;
    }
};

struct ASTMessageChainMessageNode : ASTNode
{
    ASTNodePtr selector;
    ASTNodePtrList arguments;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMessageChainMessageNode(*this);
    }

    virtual bool isMessageChainMessageNode() const override
    {
        return true;
    }
};

struct ASTMessageChainNode : ASTNode
{
    ASTNodePtr receiver;
    ASTNodePtrList messages;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMessageChainNode(*this);
    }

    virtual bool isMessageChainNode() const override
    {
        return true;
    }
};

struct ASTAssignmentNode : ASTNode
{
    ASTNodePtr variable;
    ASTNodePtr selector;
    ASTNodePtr value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitAssignmentNode(*this);
    }

    virtual bool isAssignmentNode() const override
    {
        return true;
    }
};

struct ASTReturnNode : ASTNode
{
    ASTNodePtr value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitReturnNode(*this);
    }

    virtual bool isReturnNode() const override
    {
        return true;
    }
};

struct ASTMakeArrayNode : ASTNode
{
    ASTNodePtrList elements;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMakeArrayNode(*this);
    }

    virtual bool isMakeArrayNode() const override
    {
        return true;
    }
};

struct ASTLiteralArrayNode : ASTNode
{
    ASTNodePtrList elements;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLiteralArrayNode(*this);
    }

    virtual bool isLiteralArrayNode() const override
    {
        return true;
    }
};

struct ASTLiteralByteArrayNode : ASTNode
{
    ASTNodePtrList elements;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLiteralByteArrayNode(*this);
    }

    virtual bool isLiteralByteArrayNode() const override
    {
        return true;
    }
};

inline ASTNodePtr makeParseErrorNodeAt(const TokenRange &tokens, const std::string &errorMessage)
{
    auto node = std::make_shared<ASTParseErrorNode> ();
    node->setTokenRange(tokens);
    node->errorMessage = errorMessage;
    return node;
}

inline ASTNodePtr makeParseErrorNodeAtToken(const TokenRange &position, const std::string &errorMessage)
{
    return makeParseErrorNodeAt(position.until(1), errorMessage);
}

inline ASTNodePtr makeLiteralSymbolASTNodeAt(const TokenRange &tokens, const std::string &value)
{
    auto node = std::make_shared<ASTLiteralSymbolNode> ();
    node->setTokenRange(tokens);
    node->value = value;
    return node;
}

} // End of namespace SmalltalkCompiler
} // End of namespace nyast

#endif //NYAST_SMALLTAK_COMPILER_AST_HPP
