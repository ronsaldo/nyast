#include "nyast/SmalltalkCompiler/Parser.hpp"
#include "nyast/SmalltalkCompiler/Visitors.hpp"
#include "nyast/SmalltalkCompiler/Evaluator.hpp"
#include <fstream>
#include <iostream>

using namespace nyast;

bool evaluateSmalltalkAST(const SmalltalkCompiler::ASTNodePtr &ast)
{
    if(!SmalltalkCompiler::validateASTParseErrors(ast, [&](SmalltalkCompiler::ASTParseErrorNode &parseErrorNode) {
        std::cout << parseErrorNode.sourcePosition << ": " << parseErrorNode.errorMessage << '\n';
        std::cout << parseErrorNode.sourcePosition.content() << std::endl;
    }))
        return false;

    std::cout << SmalltalkCompiler::evaluateValidatedParsedDoIt(ast) << std::endl;

    return true;
}

bool evaluateSmalltalkStreamNamed(std::istream &in, const std::string &fileName)
{
    auto content = std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());

    auto ast = SmalltalkCompiler::parseStringWithDoIt(content, fileName);
    return evaluateSmalltalkAST(ast);
}

bool evaluateSmalltalkFileNamed(const std::string &fileName)
{
    if(fileName == "-")
    {
        return evaluateSmalltalkStreamNamed(std::cin, fileName);
    }

    std::ifstream in(fileName);
    if(!in.good())
        return false;

    return evaluateSmalltalkStreamNamed(in, fileName);
}

bool evaluateSmalltalkExpression(const std::string &expression)
{
    auto ast = SmalltalkCompiler::parseStringWithDoIt(expression, "eval");
    return evaluateSmalltalkAST(ast);
}

int main(int argc, const char *argv[])
{
    return withGarbageCollectedStack([&]() {
        for(int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if(arg == "-e" || arg == "-eval")
            {
                evaluateSmalltalkExpression(argv[++i]);
            }
            else
            {
                evaluateSmalltalkFileNamed(arg);
            }
        }

        return 0;
    });
}
