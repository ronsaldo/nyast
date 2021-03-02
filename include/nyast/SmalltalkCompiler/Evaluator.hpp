#ifndef NYAST_SMALLTAK_COMPILER_EVALUATOR_HPP
#define NYAST_SMALLTAK_COMPILER_EVALUATOR_HPP
#pragma once

#include "nyast/Oop.hpp"
#include "AST.hpp"

namespace nyast
{
namespace SmalltalkCompiler
{

NYAST_CORE_EXPORT Oop evaluateValidatedParsedDoIt(ASTNodePtr ast);
NYAST_CORE_EXPORT Oop evaluateDoItString(const std::string &string, const std::string &fileName="");

} // End of namespace SmalltalkCompiler
} // End of namespace nyast

#endif //NYAST_SMALLTAK_COMPILER_EVALUATOR_HPP
