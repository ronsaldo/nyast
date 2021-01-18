#include "nyast/SmalltalkCompiler/Parser.hpp"
#include <optional>
#include <cassert>

namespace nyast
{
namespace SmalltalkCompiler
{

static ASTNodePtr parseLiteralArray(TokenRange &currentPosition);
static ASTNodePtr parseSourceWithLiteralArrayContent(TokenRange &currentPosition);
static ASTNodePtr parseLiteral(TokenRange &currentPosition);
static ASTNodePtr parseExpression(TokenRange &currentPosition);
static ASTNodePtr parseSequence(TokenRange &currentPosition);
static ASTNodePtr parsePrimaryExpression(TokenRange &currentPosition);
static ASTNodePtr parseBinaryExpression(TokenRange &currentPosition);
static ASTNodePtr parseAssignmentExpression(TokenRange &currentPosition);

static bool isBinaryOperatorToken(TokenType token)
{
    switch(token)
    {
    case TokenType::Caret:
    case TokenType::Bar:
    case TokenType::LessThan:
    case TokenType::GreaterThan:
    case TokenType::BinaryOperator:
        return true;
    default:
        return false;
    }
}
static bool isSingleKeyword(const std::string &keyword)
{
    size_t colonCount = 0;
    for(auto c : keyword)
    {
        if(c == ':')
            ++colonCount;
    }

    return colonCount == 1;
}

static ASTNodePtr parseInteger(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto literal = std::make_shared<ASTLiteralIntegerNode> ();
    auto token = currentPosition.next();
    auto position = startPosition.until(currentPosition);
    if(token.type != TokenType::Integer)
        return makeParseErrorNodeAt(position, "Expected an integer literal.");

    literal->valueString = token.text();
    literal->setTokenRange(position);
    return literal;
}

static ASTNodePtr parseFloat(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto literal = std::make_shared<ASTLiteralFloatNode> ();
    auto token = currentPosition.next();
    auto position = startPosition.until(currentPosition);
    if(token.type != TokenType::Float)
        return makeParseErrorNodeAt(position, "Expected a float point literal.");

    literal->value = atof(token.text().c_str());
    literal->setTokenRange(position);
    return literal;
}

static ASTNodePtr parseIdentifier(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto node = std::make_shared<ASTIdentifierReferenceNode> ();
    auto token = currentPosition.next();
    auto position = startPosition.until(currentPosition);
    if(token.type != TokenType::Identifier)
        return makeParseErrorNodeAt(position, "Expected an identifier reference.");

    node->identifier = token.text();
    node->setTokenRange(position);
    return node;
}

static std::string parseStringContent(const std::string &string)
{
    std::string result;
    result.reserve(string.size());

    for(size_t i = 0; i < string.size(); ++i)
    {
        auto c = string[i];
        if(c == '\'')
        {
            ++i;
            c = string[i];
        }

        result.push_back(c);
    }

    return result;
}

static ASTNodePtr parseString(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::String)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal string.");

    auto startPosition = currentPosition;
    auto rawStringValue = currentPosition.next().text();
    assert(rawStringValue.size() >= 2);
    auto rawStringContent = rawStringValue.substr(1, rawStringValue.size() - 2);

    auto node = std::make_shared<ASTLiteralStringNode> ();
    node->setTokenRange(startPosition.until(currentPosition));
    node->value = parseStringContent(rawStringContent);

    return node;
}

static ASTNodePtr parseCharacter(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::Character)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal symbol string.");

    auto startPosition = currentPosition;
    auto rawStringValue = currentPosition.next().text();
    assert(rawStringValue.size() >= 2);
    auto rawStringContent = rawStringValue.substr(1, rawStringValue.size() - 1);

    auto node = std::make_shared<ASTLiteralCharacterNode> ();
    node->setTokenRange(startPosition.until(currentPosition));

    // FIXME: Support the utf-8 encoding.
    if(rawStringContent.size() != 1)
        return makeParseErrorNodeAtToken(startPosition, "Invalid single character literal.");
    node->value = rawStringContent.front();
    return node;
}

static ASTNodePtr parseSimpleLiteralSymbol(TokenRange &currentPosition, size_t prefixSize)
{
    auto node = std::make_shared<ASTLiteralSymbolNode> ();
    node->setTokenRange(currentPosition.until(1));

    auto rawSymbolValue = currentPosition.next().text();
    assert(rawSymbolValue.size() > prefixSize);
    node->value = rawSymbolValue.substr(prefixSize);
    return node;
}

static ASTNodePtr parseLiteralSymbolKeyword(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::SymbolKeyword)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal keyword symbol.");

    return parseSimpleLiteralSymbol(currentPosition, 1);
}

static ASTNodePtr parseLiteralSymbolIdentifier(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::SymbolIdentifier)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal identifier symbol.");

    return parseSimpleLiteralSymbol(currentPosition, 1);
}

static ASTNodePtr parseLiteralSymbolOperator(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::SymbolOperator)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal operator symbol.");

    return parseSimpleLiteralSymbol(currentPosition, 1);
}

static ASTNodePtr parseLiteralSymbolString(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::SymbolString)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal symbol string.");

    auto startPosition = currentPosition;
    auto rawStringValue = currentPosition.next().text();
    assert(rawStringValue.size() >= 3);
    auto rawStringContent = rawStringValue.substr(2, rawStringValue.size() - 3);

    auto node = std::make_shared<ASTLiteralSymbolNode> ();
    node->setTokenRange(startPosition.until(currentPosition));
    node->value = parseStringContent(rawStringContent);

    return node;
}

static ASTNodePtr parseLiteralArrayElement(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::LeftParent: return parseLiteralArray(currentPosition);
    case TokenType::Keyword: return parseSimpleLiteralSymbol(currentPosition, 0);
    case TokenType::Identifier: return parseIdentifier(currentPosition);
    case TokenType::Colon:
    case TokenType::ColonColon:
    case TokenType::Dot:
    case TokenType::LessThan:
    case TokenType::GreaterThan:
    case TokenType::Bar:
    case TokenType::BinaryOperator:
        return parseSimpleLiteralSymbol(currentPosition, 0);
    default: return parseLiteral(currentPosition);
    }
}

static ASTNodePtr parseLiteralArray(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    {
        auto type = currentPosition.peek().type;
        if(type != TokenType::LiteralArrayLeftParent && type != TokenType::LeftParent)
            return makeParseErrorNodeAtToken(currentPosition, "Expected a literal array.");
        currentPosition.advance();
    }

    auto literalArray = std::make_shared<ASTLiteralArrayNode> ();
    while(currentPosition.peek().type != TokenType::RightParent)
    {
        auto element = parseLiteralArrayElement(currentPosition);
        literalArray->elements.push_back(element);
        if(element->isParseErrorNode())
        {
            literalArray->setTokenRange(startPosition.until(currentPosition));
            return literalArray;
        }
    }

    if(currentPosition.peek().type != TokenType::RightParent)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a right parenthesis.");
    currentPosition.advance();
    literalArray->setTokenRange(startPosition.until(currentPosition));
    return literalArray;
}

static ASTNodePtr parseLiteralByteArray(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    {
        auto type = currentPosition.peek().type;
        if(type != TokenType::ByteArrayLeftBracket)
            return makeParseErrorNodeAtToken(currentPosition, "Expected a literal byte array.");
        currentPosition.advance();
    }

    auto literalByteArray = std::make_shared<ASTLiteralByteArrayNode> ();
    while(currentPosition.peek().type != TokenType::RightBracket)
    {
        auto element = parseInteger(currentPosition);
        literalByteArray->elements.push_back(element);
        if(element->isParseErrorNode())
        {
            literalByteArray->setTokenRange(startPosition.until(currentPosition));
            return literalByteArray;
        }
    }

    if(currentPosition.peek().type != TokenType::RightBracket)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a right bracket.");
    currentPosition.advance();
    literalByteArray->setTokenRange(startPosition.until(currentPosition));
    return literalByteArray;
}

static ASTNodePtr parseSourceWithLiteralArrayContent(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;

    auto literalArray = std::make_shared<ASTLiteralArrayNode> ();
    while(currentPosition.peek().type != TokenType::EndOfSource)
    {
        auto element = parseLiteralArrayElement(currentPosition);
        literalArray->elements.push_back(element);
        if(element->isParseErrorNode())
        {
            literalArray->setTokenRange(startPosition.until(currentPosition));
            return literalArray;
        }
    }

    if(currentPosition.peek().type != TokenType::EndOfSource)
        return makeParseErrorNodeAtToken(currentPosition, "Expected the end of source.");
    currentPosition.advance();
    literalArray->setTokenRange(startPosition.until(currentPosition));
    return literalArray;
}

static ASTNodePtr parseLiteral(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::Integer: return parseInteger(currentPosition);
    case TokenType::Float: return parseFloat(currentPosition);
    case TokenType::String: return parseString(currentPosition);
    case TokenType::Character: return parseCharacter(currentPosition);
    case TokenType::SymbolKeyword: return parseLiteralSymbolKeyword(currentPosition);
    case TokenType::SymbolIdentifier: return parseLiteralSymbolIdentifier(currentPosition);
    case TokenType::SymbolOperator: return parseLiteralSymbolOperator(currentPosition);
    case TokenType::SymbolString: return parseLiteralSymbolString(currentPosition);
    case TokenType::LiteralArrayLeftParent: return parseLiteralArray(currentPosition);
    case TokenType::ByteArrayLeftBracket: return parseLiteralByteArray(currentPosition);
    default:
        return makeParseErrorNodeAtToken(currentPosition, "Expected a literal.");
    }
}

static ASTNodePtr parseParentExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::LeftParent)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a left parenthesis.");
    currentPosition.advance();

    auto expression = parseExpression(currentPosition);
    if(expression->isParseErrorNode())
        return expression;

    if(currentPosition.peek().type != TokenType::RightParent)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a right parenthesis.");
    currentPosition.advance();
    return expression;
}

static ASTNodePtr parseMakeArrayExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::LeftCurlyBracket)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a left curly bracket.");
    currentPosition.advance();

    auto result = std::make_shared<ASTMakeArrayNode> ();
    do
    {
        if(currentPosition.peek().type == TokenType::Dot)
            currentPosition.advance();

        auto nextTokenType = currentPosition.peek().type;
        if(nextTokenType != TokenType::Dot && nextTokenType != TokenType::RightCurlyBracket)
        {
            auto expression = parseExpression(currentPosition);
            result->elements.push_back(expression);
            if(expression->isParseErrorNode())
                return expression;
        }
    } while(currentPosition.peek().type == TokenType::Dot);


    if(currentPosition.peek().type != TokenType::RightCurlyBracket)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a right curly bracket.");
    currentPosition.advance();
    result->setTokenRange(startPosition.until(currentPosition));
    return result;
}

static void parseKeywordPragmaContent(TokenRange &currentPosition, ASTPragmaNode &pragma)
{
    std::string selectorValue;
    auto selectorStartPosition = currentPosition;

    // Parse the pragme keyword and arguments.
    while(currentPosition.peek().type == TokenType::Keyword)
    {
        auto keywordPosition = currentPosition.until(1);
        auto keyword = currentPosition.next().text();
        selectorValue += keyword;

        if(!isSingleKeyword(keyword))
            pragma.arguments.push_back(makeParseErrorNodeAtToken(keywordPosition, "Expected a single keyword."));

        auto argument = parsePrimaryExpression(currentPosition);
        pragma.arguments.push_back(argument);
        if(argument->isParseErrorNode())
            break;
    }

    // Set the selector value.
    auto selector = std::make_shared<ASTLiteralSymbolNode> ();
    selector->value = selectorValue;
    selector->setTokenRange(selectorStartPosition.until(currentPosition));
    pragma.selector = selector;
}

static ASTNodePtr parsePragma(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::LessThan)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a pragma.");
    currentPosition.advance();

    auto pragma = std::make_shared<ASTPragmaNode> ();
    switch(currentPosition.peek().type)
    {
    case TokenType::Identifier:
        {
            auto selector = std::make_shared<ASTLiteralSymbolNode> ();
            selector->setTokenRange(currentPosition.until(1));
            selector->value = currentPosition.next().text();
            pragma->selector = selector;
        }
        break;
    case TokenType::Keyword:
        parseKeywordPragmaContent(currentPosition, *pragma);
        break;
    default:
        return makeParseErrorNodeAtToken(currentPosition, "Expected a pragma selector.");
    }

    if(currentPosition.peek().type == TokenType::GreaterThan)
    {
        currentPosition.advance();
    }
    else
    {
        if(pragma->arguments.empty() || !pragma->arguments.back()->isParseErrorNode())
            pragma->arguments.push_back(makeParseErrorNodeAtToken(currentPosition, "Expected the pragma end delimiter '>'."));
    }

    return pragma;
}

static ASTNodePtr parseBlockClosureArgument(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::Colon)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a colon (:) before a block closure argument definition.");
    currentPosition.advance();

    if(currentPosition.peek().type != TokenType::Identifier)
        return makeParseErrorNodeAtToken(currentPosition, "Expected an identifier for the block closure argument definition.");

    auto node = std::make_shared<ASTArgumentNode> ();
    node->identifier = currentPosition.next().text();
    node->setTokenRange(startPosition.until(currentPosition));
    return node;
}

static ASTNodePtr parseBlockClosureExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::LeftBracket)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a block starting with a left curly bracket ('{').");
    currentPosition.advance();

    // Create the block node.
    auto block = std::make_shared<ASTBlockClosureNode> ();

    // Parse the block closure arguments.
    while(currentPosition.peek().type == TokenType::Colon)
    {
        auto argument = parseBlockClosureArgument(currentPosition);
        block->arguments.push_back(argument);
        if(argument->isParseErrorNode())
            break;
    }

    // Skip the bar.
    if(currentPosition.peek().type == TokenType::Bar)
        currentPosition.advance();

    // Parse the block expression list.
    block->sequence = parseSequence(currentPosition);
    if(currentPosition.peek().type == TokenType::RightBracket)
    {
        currentPosition.advance();
    }
    else
    {
        if(!block->sequence->isParseErrorNode())
            block->sequence = makeParseErrorNodeAtToken(currentPosition, "Expected a block ending with a right curly bracket ('}').");
    }

    block->setTokenRange(startPosition.until(currentPosition));
    return block;
}

static ASTNodePtr parsePrimaryExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    switch(currentPosition.peek().type)
    {
    case TokenType::LeftParent: return parseParentExpression(currentPosition);
    case TokenType::LeftCurlyBracket: return parseMakeArrayExpression(currentPosition);
    case TokenType::LeftBracket: return parseBlockClosureExpression(currentPosition);
    case TokenType::Identifier: return parseIdentifier(currentPosition);
    default:
        return parseLiteral(currentPosition);
    }
}

static ASTNodePtr parseUnaryPostfixExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto receiver = parsePrimaryExpression(currentPosition);
    if(receiver->isParseErrorNode())
        return receiver;

    for(;;)
    {
        switch(currentPosition.peek().type)
        {
        case TokenType::Identifier:
            // Unary message
            {
                auto selector = std::make_shared<ASTLiteralSymbolNode> ();
                selector->setTokenRange(currentPosition.until(1));
                selector->value = currentPosition.next().text();

                auto messageSend = std::make_shared<ASTMessageSendNode> ();
                messageSend->setTokenRange(startPosition.until(currentPosition));
                messageSend->receiver = receiver;
                messageSend->selector = selector;
                receiver = messageSend;
            }
            break;
        default:
            return receiver;
        }
    }
}

static ASTNodePtr parseBinaryExpression(TokenRange &currentPosition)
{
    auto receiver = parseUnaryPostfixExpression(currentPosition);
    if(receiver->isParseErrorNode())
        return receiver;

    while(isBinaryOperatorToken(currentPosition.peek().type))
    {
        // Make the selector symbol node.
        auto selectorPosition = currentPosition.until(1);
        auto operatorToken = currentPosition.next();
        auto selectorNode = makeLiteralSymbolASTNodeAt(selectorPosition, operatorToken.text());

        // Parse the next operand
        auto rightOperand = parseUnaryPostfixExpression(currentPosition);

        // Make the node.
        auto messageSend = std::make_shared<ASTMessageSendNode> ();
        messageSend->setTokenRange(receiver->tokens.until(rightOperand->tokens));
        messageSend->selector = selectorNode;
        messageSend->receiver = receiver;
        messageSend->arguments.push_back(rightOperand);
        receiver = messageSend;
    }

    return receiver;
}

static ASTNodePtr parseChainReceiver(TokenRange &currentPosition)
{
    return parseBinaryExpression(currentPosition);
}

static ASTNodePtr parseChainKeywordMessage(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::Keyword)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a keyword chained message.");

    std::string selectorContent;
    auto chainMessage = std::make_shared<ASTMessageChainMessageNode> ();

    while(currentPosition.peek().type == TokenType::Keyword)
    {
        auto keywordPosition = currentPosition.until(1);
        auto keyword = currentPosition.next().text();
        selectorContent += keyword;

        if(!isSingleKeyword(keyword))
            chainMessage->arguments.push_back(makeParseErrorNodeAtToken(keywordPosition, "Expected a single keyword."));

        auto argument = parseBinaryExpression(currentPosition);
        chainMessage->arguments.push_back(argument);
        if(argument->isParseErrorNode())
            break;
    }

    auto selector = std::make_shared<ASTLiteralSymbolNode> ();
    selector->value = selectorContent;
    selector->setTokenRange(startPosition.until(currentPosition));

    chainMessage->selector = selector;
    chainMessage->setTokenRange(startPosition.until(currentPosition));

    return chainMessage;
}

static ASTNodePtr parseChainUnaryMessage(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::Identifier)
        return makeParseErrorNodeAtToken(currentPosition, "Expected an unary chained message.");

    auto position = currentPosition.until(1);
    auto selector = std::make_shared<ASTLiteralSymbolNode> ();
    selector->value = currentPosition.next().text();
    selector->setTokenRange(position);

    auto messageNode = std::make_shared<ASTMessageChainMessageNode> ();
    messageNode->selector = selector;
    messageNode->setTokenRange(position);
    return messageNode;
}

static ASTNodePtr parseChainedMessage(TokenRange &currentPosition)
{
    switch(currentPosition.peek().type)
    {
    case TokenType::Keyword: return parseChainKeywordMessage(currentPosition);
    case TokenType::Identifier: return parseChainUnaryMessage(currentPosition);
    default: return makeParseErrorNodeAtToken(currentPosition, "Expected a chained keyword or unary message.");
    }
}

static ASTNodePtr parseChainExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto receiver = ASTNodePtr();

    if(currentPosition.peek().type != TokenType::Keyword)
    {
        receiver = parseChainReceiver(currentPosition);
        auto nextType = currentPosition.peek().type;
        if(nextType != TokenType::Keyword && nextType != TokenType::Semicolon)
            return receiver;
    }

    ASTNodePtr firstChainKeywordMessage;
    if(currentPosition.peek().type == TokenType::Keyword)
    {
        firstChainKeywordMessage = parseChainKeywordMessage(currentPosition);
    }


    // Not chained message case.
    if(currentPosition.peek().type != TokenType::Semicolon)
    {
        if(!firstChainKeywordMessage)
            return receiver;

        assert(firstChainKeywordMessage->isMessageChainMessageNode());
        auto &castedChain = firstChainKeywordMessage->as<ASTMessageChainMessageNode> ();

        auto messageNode = std::make_shared<ASTMessageSendNode> ();
        messageNode->setTokenRange(startPosition.until(currentPosition));
        messageNode->receiver = receiver;
        messageNode->selector = castedChain.selector;
        messageNode->arguments = castedChain.arguments;
        return messageNode;
    }

    // Message chain case.
    auto chainNode = std::make_shared<ASTMessageChainNode> ();

    if(firstChainKeywordMessage)
    {
        chainNode->receiver = receiver;
        chainNode->messages.push_back(firstChainKeywordMessage);
    }
    else
    {
        // If the receiver is a message send node, convert split it into two parts.
        if(receiver->isMessageSendNode())
        {
            auto &firstMessage = receiver->as<ASTMessageSendNode> ();
            chainNode->receiver = firstMessage.receiver;

            auto firstChain = std::make_shared<ASTMessageChainMessageNode> ();
            firstChain->tokens = receiver->tokens;
            firstChain->sourcePosition = receiver->sourcePosition;
            firstChain->selector = firstMessage.selector;
            firstChain->arguments = firstMessage.arguments;
            chainNode->messages.push_back(firstChain);
        }
        else
        {
            chainNode->receiver = receiver;
        }
    }
    while(currentPosition.peek().type == TokenType::Semicolon)
    {
        currentPosition.advance();

        auto chainedMesage = parseChainedMessage(currentPosition);
        chainNode->messages.push_back(chainedMesage);
    }

    chainNode->setTokenRange(startPosition.until(currentPosition));
    return chainNode;
}

static ASTNodePtr parseAssignmentExpression(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    auto variable = parseChainExpression(currentPosition);
    if(currentPosition.peek().type != TokenType::Assignment)
        return variable;

    auto selector = std::make_shared<ASTLiteralSymbolNode> ();
    selector->setTokenRange(currentPosition.until(1));
    selector->value = ":=";
    currentPosition.advance();

    auto assignment = std::make_shared<ASTAssignmentNode> ();
    assignment->variable = variable;
    assignment->selector = selector;
    assignment->value = parseAssignmentExpression(currentPosition);
    assignment->setTokenRange(currentPosition.until(startPosition));
    return assignment;
}

static ASTNodePtr parseReturnExpression(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::Caret)
        return parseAssignmentExpression(currentPosition);

    auto startPosition = currentPosition;
    currentPosition.advance();

    auto returnNode = std::make_shared<ASTReturnNode> ();
    returnNode->value = parseAssignmentExpression(currentPosition);
    returnNode->setTokenRange(startPosition.until(currentPosition));
    return returnNode;
}

static ASTNodePtr parseExpression(TokenRange &currentPosition)
{
    return parseReturnExpression(currentPosition);
}

static ASTNodePtr parseLocalDeclaration(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::Identifier)
        return makeParseErrorNodeAtToken(currentPosition, "Expected an identifier.");

    auto declaration = std::make_shared<ASTLocalDeclarationNode> ();
    declaration->setTokenRange(currentPosition.until(1));
    declaration->identifier = currentPosition.next().text();
    return declaration;
}

static ASTNodePtr parseLocalDeclarations(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    if(currentPosition.peek().type != TokenType::Bar)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a bar.");
    currentPosition.advance();

    auto localDeclarations = std::make_shared<ASTLocalDeclarationsNode> ();
    while(currentPosition.peek().type != TokenType::Bar)
    {
        auto localDeclaration = parseLocalDeclaration(currentPosition);
        localDeclarations->declarations.push_back(localDeclaration);
        if(localDeclaration->isParseErrorNode())
            return localDeclarations;
    }

    auto expression = parseExpression(currentPosition);
    if(expression->isParseErrorNode())
        return expression;

    if(currentPosition.peek().type != TokenType::Bar)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a bar.");
    currentPosition.advance();
    localDeclarations->setTokenRange(startPosition.until(currentPosition));
    return localDeclarations;
}

static ASTNodePtr parseSequence(TokenRange &currentPosition)
{
    auto result = std::make_shared<ASTSequenceNode> ();
    auto startPosition = currentPosition;

    // Parse local declarations before the pragmas.
    if(currentPosition.peek().type == TokenType::Bar)
        result->localDeclarations = parseLocalDeclarations(currentPosition);

    // Parse the pragmas.
    while(currentPosition.peek().type == TokenType::LessThan)
    {
        auto pragma = parsePragma(currentPosition);
        result->pragmas.push_back(pragma);
        if(pragma->isParseErrorNode())
            break;
    }

    // Parse local declarations after the pragma.
    if(!result->localDeclarations && currentPosition.peek().type == TokenType::Bar)
        result->localDeclarations = parseLocalDeclarations(currentPosition);

    do
    {
        if(currentPosition.peek().type == TokenType::Dot)
            currentPosition.next();

        auto memento = currentPosition;
        auto expression = parseExpression(currentPosition);
        if(!expression->isParseErrorNode())
            result->expressions.push_back(expression);
        else
            currentPosition = memento;
    } while(currentPosition.peek().type == TokenType::Dot);
    result->tokens = startPosition.until(currentPosition);

    return result;
}

static ASTNodePtr parseUnaryMethodHeader(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::Identifier)
        return makeParseErrorNodeAtToken(currentPosition, "Expected an unary method selector.");

    auto startPosition = currentPosition;
    auto selector = std::make_shared<ASTLiteralSymbolNode> ();
    selector->value = currentPosition.next().text();
    selector->setTokenRange(startPosition.until(currentPosition));

    auto method = std::make_shared<ASTMethodNode> ();
    method->selector = selector;
    return method;
}

static ASTNodePtr parseBinaryMethodHeader(TokenRange &currentPosition)
{
    if(!isBinaryOperatorToken(currentPosition.peek().type))
        return makeParseErrorNodeAtToken(currentPosition, "Expected a binary method selector.");

    auto selectorStartPosition = currentPosition;
    auto selector = std::make_shared<ASTLiteralSymbolNode> ();
    selector->value = currentPosition.next().text();
    selector->setTokenRange(selectorStartPosition.until(currentPosition));

    if(currentPosition.peek().type != TokenType::Identifier)
        return makeParseErrorNodeAtToken(currentPosition, "Expected the binary method argument name.");

    auto argumentStartPosition = currentPosition;
    auto argument = std::make_shared<ASTArgumentNode> ();
    argument->identifier = currentPosition.next().text();
    argument->setTokenRange(argumentStartPosition.until(currentPosition));

    auto method = std::make_shared<ASTMethodNode> ();
    method->selector = selector;
    method->arguments.push_back(argument);
    return method;
}

static ASTNodePtr parseKeywordMethodHeader(TokenRange &currentPosition)
{
    if(currentPosition.peek().type != TokenType::Keyword)
        return makeParseErrorNodeAtToken(currentPosition, "Expected a keyword method selector.");

    auto selectorStartPosition = currentPosition;
    std::string selectorContent;
    auto method = std::make_shared<ASTMethodNode> ();

    while(currentPosition.peek().type == TokenType::Keyword)
    {
        selectorContent += currentPosition.next().text();

        if(currentPosition.peek().type != TokenType::Identifier)
        {
            method->arguments.push_back(makeParseErrorNodeAtToken(currentPosition, "Expected the keyworrd method argument name."));
            break;
        }
        else
        {
            auto argumentStartPosition = currentPosition;
            auto argument = std::make_shared<ASTArgumentNode> ();
            argument->identifier = currentPosition.next().text();
            argument->setTokenRange(argumentStartPosition.until(currentPosition));
            method->arguments.push_back(argument);
        }
    }

    auto selector = std::make_shared<ASTLiteralSymbolNode> ();
    selector->value = selectorContent;
    selector->setTokenRange(selectorStartPosition.until(currentPosition));

    method->selector = selector;
    return method;
}

static ASTNodePtr parseMethod(TokenRange &currentPosition)
{
    auto startPosition = currentPosition;
    ASTNodePtr method;
    switch(currentPosition.peek().type)
    {
    case TokenType::Identifier:
        method = parseUnaryMethodHeader(currentPosition);
        break;
    case TokenType::Keyword:
        method = parseKeywordMethodHeader(currentPosition);
        break;
    default:
        if(isBinaryOperatorToken(currentPosition.peek().type))
            method = parseBinaryMethodHeader(currentPosition);
        else
            return makeParseErrorNodeAt(currentPosition, "Expected a method header.");
    }

    if(method->isMethodNode())
    {
        method->as<ASTMethodNode> ().sequence = parseSequence(currentPosition);
        method->setTokenRange(startPosition.until(currentPosition));
    }

    return method;
}

ASTNodePtr parseTokenListWithDoIt(const TokenListPtr &tokenList)
{
    auto currentPosition = TokenRange::forCollection(tokenList);
    auto result = parseSequence(currentPosition);
    if(result->isParseErrorNode())
        return result;
    if(!currentPosition.peek().isEndOfSource())
        result->as<ASTSequenceNode> ().expressions.push_back(makeParseErrorNodeAtToken(currentPosition, "Expected the end of the source code."));
    return result;
}

ASTNodePtr parseTokenListWithMethod(const TokenListPtr &tokenList)
{
    auto currentPosition = TokenRange::forCollection(tokenList);
    auto result = parseMethod(currentPosition);
    if(result->isParseErrorNode() || result->as<ASTMethodNode> ().sequence->isParseErrorNode())
        return result;
    if(!currentPosition.peek().isEndOfSource())
        result->as<ASTMethodNode> ()
            .sequence->as<ASTSequenceNode> ()
                .expressions.push_back(makeParseErrorNodeAtToken(currentPosition, "Expected the end of the source code."));
    return result;

    return parseMethod(currentPosition);
}

ASTNodePtr parseTokenListWithLiteralArrayContent(const TokenListPtr &tokenList)
{
    auto currentPosition = TokenRange::forCollection(tokenList);
    return parseSourceWithLiteralArrayContent(currentPosition);
}

} // End of namespace SmalltalkCompiler
} // End of namespace nyast
