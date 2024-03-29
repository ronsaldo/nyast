#include "nyast/SmalltalkCompiler/Scanner.hpp"
#include <unordered_map>
#include <cassert>

namespace nyast
{
namespace SmalltalkCompiler
{


static bool skipMultiLineComment(SourcePosition &currentPosition)
{
    assert(currentPosition.peek() == '"');
    currentPosition.advance();
    while(!currentPosition.empty())
    {
        auto next = currentPosition.next();
        if(next == '"')
        {
            return true;
        }
    }

    return false;
}

static bool skipWhite(SourcePosition &currentPosition)
{
    while(!currentPosition.empty())
    {
        auto next = currentPosition.peek(0);
        if(next <= ' ')
        {
            currentPosition.advance();
            continue;
        }

        if(next == '"')
        {
            auto memento = currentPosition;
            if(!skipMultiLineComment(currentPosition))
            {
                currentPosition = memento;
                return false;
            }
            continue;
        }

        return true;
    }

    return true;
}

static inline bool isDigit(int c)
{
    return '0' <= c && c <= '9';
}

static inline bool isSign(int c)
{
    return '-' == c;
}

static inline bool isExponent(int c)
{
    return 'e' == c || 'E' == c;
}

static inline bool isRadix(int c)
{
    return 'r' == c || 'R' == c;
}

static inline bool isAlpha(int c)
{
    return ('A' <= c && c <= 'Z') ||
        ('a' <= c && c <= 'z');
}

static inline bool isAlphaNumeric(int c)
{
    return isAlpha(c) || isDigit(c);
}

static inline bool isIdentifierStart(int c)
{
    return isAlpha(c) ||
        '_' == c;
}

static inline bool isIdentifierCharacter(int c)
{
    return isIdentifierStart(c) || isDigit(c);
}

static inline bool isOperatorCharacter(int c)
{
    switch(c)
    {
    case '+':
    case '-':
    case '/':
    case '\\':
    case '*':
    case '~':
    case '<':
    case '>':
    case '=':
    case '@':
    case ',':
    case '%':
    case '|':
    case '&':
    case '?':
    case '!':
    case '^':
        return true;
    default:
        return false;
    }
}

static TokenType skipKeywordOrIdentifier(SourcePosition &currentPosition)
{
    bool scanningKeyword = false;
    auto lastKeywordEndPosition = currentPosition;

    do
    {
        currentPosition.skipWhile(isIdentifierCharacter);
        if(currentPosition.peek() == ':' && currentPosition.peek(1) != '=')
        {
            currentPosition.advance();
            scanningKeyword = true;
            lastKeywordEndPosition = currentPosition;
        }
        else
        {
            if(scanningKeyword)
            {
                currentPosition = lastKeywordEndPosition;
                return TokenType::Keyword;
            }
        }

    } while(scanningKeyword && isIdentifierStart(currentPosition.peek()));

    return scanningKeyword ? TokenType::Keyword : TokenType::Identifier;
}

static TokenType skipNumber(SourcePosition &currentPosition)
{
    // Skip the sign;
    if(isSign(currentPosition.peek()))
        currentPosition.advance();

    // Skip the integer or radix part.
    currentPosition.skipWhile(isDigit);

    auto dotExponentOrRadix = currentPosition.peek();
    if(isRadix(dotExponentOrRadix))
    {
        // Radix integer
        currentPosition.skipWhile(isAlphaNumeric);
        return TokenType::Integer;
    }

    bool isFloat = false;

    // Fractional part.
    if(dotExponentOrRadix == '.' && isDigit(currentPosition.peek(1)))
    {
        currentPosition.advance();
        isFloat = true;
        currentPosition.skipWhile(isDigit);

        dotExponentOrRadix = currentPosition.peek();
    }

    // Float exponent.
    if(isExponent(dotExponentOrRadix))
    {
        currentPosition.advance();
        isFloat = true;
        if(isSign(currentPosition.peek()))
            currentPosition.advance();

        currentPosition.skipWhile(isDigit);
    }

    return isFloat ? TokenType::Float : TokenType::Integer;
}

static bool skipStringLiteral(SourcePosition &currentPosition)
{
    assert(currentPosition.peek() == '\'');
    currentPosition.advance();

    while(currentPosition.peek() != '\''
        || (currentPosition.peek() == '\'' && currentPosition.peek(1) == '\''))
    {
        if(currentPosition.empty())
            return false;

        if(currentPosition.next() == '\'')
        {
            assert(currentPosition.peek() == '\'');
            currentPosition.advance();
        }
    }

    if(currentPosition.peek() != '\'')
        return false;

    currentPosition.advance();
    return true;
}

static void scanNextToken(SourcePosition &currentPosition, TokenList &result)
{
    if(!skipWhite(currentPosition))
    {
        // This may fail due to an unclosed multiline comment.
        result.push_back(Token {TokenType::ErrorIncompleteComment, currentPosition});
        currentPosition.advanceToEnd();
        return;
    }

    if(currentPosition.empty())
    {
        result.push_back(Token {TokenType::EndOfSource, currentPosition});
        return;
    }

    auto tokenStart = currentPosition;
    auto tokenFirst = currentPosition.peek();

    // Special operators starting with a colon.
    if(tokenFirst == ':')
    {
        auto tokenType = TokenType::Colon;
        currentPosition.advance();

        switch(currentPosition.peek())
        {
        case ':':
            tokenType = TokenType::ColonColon;
            currentPosition.advance();
            break;
        case '=':
            tokenType = TokenType::Assignment;
            currentPosition.advance();
            break;
        default:
            // Just the colon.
            break;
        }

        result.push_back(Token {tokenType, tokenStart.until(currentPosition)});
        return;
    }

    // Symbols
    if(tokenFirst == '#')
    {
        auto symbolFirst = currentPosition.peek(1);
        if(isIdentifierStart(symbolFirst))
        {
            currentPosition.advance();
            auto tokenType = skipKeywordOrIdentifier(currentPosition);
            switch(tokenType)
            {
            case TokenType::Identifier:
                tokenType = TokenType::SymbolIdentifier;
                break;
            case TokenType::Keyword:
                tokenType = TokenType::SymbolKeyword;
                break;
            default:
                abort();
                break;
            }
            result.push_back(Token {tokenType, tokenStart.until(currentPosition)});
            return;
        }
        else if(isOperatorCharacter(symbolFirst))
        {
            currentPosition.advance();
            currentPosition.skipWhile(isOperatorCharacter);
            result.push_back(Token {TokenType::SymbolOperator, tokenStart.until(currentPosition)});
            return;
        }

        switch(symbolFirst)
        {
        case '\'':
            currentPosition.advance();
            if(skipStringLiteral(currentPosition))
                result.push_back(Token {TokenType::SymbolString, tokenStart.until(currentPosition)});
            else
                result.push_back(Token {TokenType::ErrorIncompleteSymbolString, tokenStart.until(currentPosition)});
            return;
        case '(':
            currentPosition.advance(2);
            result.push_back(Token {TokenType::LiteralArrayLeftParent, tokenStart.until(currentPosition)});
            return;
        case '[':
            currentPosition.advance(2);
            result.push_back(Token {TokenType::ByteArrayLeftBracket, tokenStart.until(currentPosition)});
            return;
        default:
            break;
        }
    }

    // Numbers
    if(isDigit(tokenFirst) || (isSign(tokenFirst) && isDigit(currentPosition.peek(1))))
    {
        auto tokenType = skipNumber(currentPosition);
        result.push_back(Token {tokenType, tokenStart.until(currentPosition)});
        return;
    }

    // Identifiers and keywords.
    if(isIdentifierStart(tokenFirst))
    {
        auto tokenType = skipKeywordOrIdentifier(currentPosition);
        result.push_back(Token {tokenType, tokenStart.until(currentPosition)});
        return;
    }

    // String literal.
    if(tokenFirst == '\'')
    {
        if(skipStringLiteral(currentPosition))
            result.push_back(Token {TokenType::String, tokenStart.until(currentPosition)});
        else
            result.push_back(Token {TokenType::ErrorIncompleteString, tokenStart.until(currentPosition)});
        return;
    }

    // Character literal.
    if(tokenFirst == '$')
    {
        currentPosition.advance();
        if(currentPosition.empty())
        {
            result.push_back(Token {TokenType::ErrorIncompleteCharacter, tokenStart.until(currentPosition)});
            return;
        }

        currentPosition.advance();
        result.push_back(Token {TokenType::Character, tokenStart.until(currentPosition)});
        return;
    }

    // Operators
    if(isOperatorCharacter(tokenFirst))
    {
        currentPosition.skipWhile(isOperatorCharacter);
        auto operatorSourcePosition = tokenStart.until(currentPosition);
        auto tokenType = TokenType::BinaryOperator;
        if(operatorSourcePosition.size() == 1)
        {
            switch(operatorSourcePosition.front())
            {
            case '^':
                tokenType = TokenType::Caret;
                break;
            case '|':
                tokenType = TokenType::Bar;
                break;
            case '<':
                tokenType = TokenType::LessThan;
                break;
            case '>':
                tokenType = TokenType::GreaterThan;
                break;
            default:
                break;
            }
        }
        result.push_back(Token {tokenType, operatorSourcePosition});
        return;
    }

    // Other delimiters
    TokenType delimiterTokenType = TokenType::Error;
    currentPosition.advance();
    switch(tokenFirst)
    {
    case '.':
        delimiterTokenType = TokenType::Dot;
        break;
    case ';':
        delimiterTokenType = TokenType::Semicolon;
        break;
    case '(':
        delimiterTokenType = TokenType::LeftParent;
        break;
    case ')':
        delimiterTokenType = TokenType::RightParent;
        break;
    case '[':
        delimiterTokenType = TokenType::LeftBracket;
        break;
    case ']':
        delimiterTokenType = TokenType::RightBracket;
        break;
    case '{':
        delimiterTokenType = TokenType::LeftCurlyBracket;
        break;
    case '}':
        delimiterTokenType = TokenType::RightCurlyBracket;
        break;
    default:
        // Unrecognized token. Attempt to merge with the last error.
        if(!result.empty() && result.back().isErrorUnexpected())
        {
            currentPosition.advance(1);
            auto &lastErrorPosition = result.back().sourcePosition;
            lastErrorPosition.endPosition = std::max(lastErrorPosition.endPosition, currentPosition.startPosition);
            return;
        }
        break;
    }

    result.push_back(Token {delimiterTokenType, tokenStart.until(currentPosition)});

}

TokenListPtr scanSourceCollection(const SourceCollectionPtr &sourceCollection)
{
    auto result = std::make_shared<TokenList> ();
    auto currentPosition = SourcePosition::forCollection(sourceCollection);
    do
    {
        scanNextToken(currentPosition, *result);
    } while (!currentPosition.empty());

    return result;
}

} // End of namespace SmalltalkCompiler
} // End of namespace nyast
