#include "nyast/SmalltalkCompiler/Parser.hpp"
#include "nyast/SmalltalkCompiler/Visitors.hpp"
#include "nyast/SmalltalkCompiler/Interpreter.hpp"
#include <fstream>
#include <iostream>

using namespace nyast;

bool evaluateSmalltalkStreamNamed(std::istream &in, const std::string &fileName)
{
    auto content = std::string(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());

    auto ast = SmalltalkCompiler::parseStringWithDoIt(content, fileName);
    SmalltalkCompiler::validateASTParseErrors(ast, [&](SmalltalkCompiler::ASTParseErrorNode &parseErrorNode) {
        std::cout << parseErrorNode.sourcePosition << ": " << parseErrorNode.errorMessage << '\n';
        std::cout << parseErrorNode.sourcePosition.content() << std::endl;
    });

    //auto result = evaluateSmalltalkScriptAST(ast, createDefaultBootstrapEvaluationEnvironmentFor(environment));
    //std::cout << result->asString() << std::endl;

    return true;
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

int main(int argc, const char *argv[])
{
    for(int i = 1; i < argc; ++i)
    {
        evaluateSmalltalkFileNamed(argv[i]);
    }

    return 0;
}
