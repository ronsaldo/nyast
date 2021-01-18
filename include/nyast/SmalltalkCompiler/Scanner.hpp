#ifndef NYAST_SMALLTAK_COMPILER_SCANNER_HPP
#define NYAST_SMALLTAK_COMPILER_SCANNER_HPP
#pragma once

#include "Token.hpp"

namespace nyast
{
namespace SmalltalkCompiler
{


TokenListPtr scanSourceCollection(const SourceCollectionPtr &sourceCollection);

inline TokenListPtr scanString(const std::string &sourceString, const std::string &sourceName = "")
{
    return scanSourceCollection(std::make_shared<SourceCollection> (sourceString, sourceName));
}

} // End of namespace SmalltalkCompiler
} // End of namespace nyast

#endif //NYAST_SMALLTAK_COMPILER_SCANNER_HPP
