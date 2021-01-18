#ifndef NYAST_SMALLTAK_COMPILER_PARSER_HPP
#define NYAST_SMALLTAK_COMPILER_PARSER_HPP
#pragma once

#include "AST.hpp"
#include "Scanner.hpp"

namespace nyast
{
namespace SmalltalkCompiler
{

ASTNodePtr parseTokenListWithDoIt(const TokenListPtr &tokenList);
ASTNodePtr parseTokenListWithMethod(const TokenListPtr &tokenList);
ASTNodePtr parseTokenListWithLiteralArrayContent(const TokenListPtr &tokenList);

inline ASTNodePtr parseStringWithDoIt(const std::string &sourceString, const std::string &sourceName = "")
{
    return parseTokenListWithDoIt(scanString(sourceString, sourceName));
}

inline ASTNodePtr parseStringWithMethod(const std::string &sourceString, const std::string &sourceName = "")
{
    return parseTokenListWithMethod(scanString(sourceString, sourceName));
}

inline ASTNodePtr parseStringWithLiteralArrayContent(const std::string &sourceString, const std::string &sourceName = "")
{
    return parseTokenListWithLiteralArrayContent(scanString(sourceString, sourceName));
}

} // End of namespace SmalltalkCompiler
} // End of namespace nyast

#endif //NYAST_SMALLTAK_COMPILER_PARSER_HPP
