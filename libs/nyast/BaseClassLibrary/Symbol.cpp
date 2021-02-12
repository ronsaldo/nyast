#include "nyast/BaseClassLibrary/Symbol.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{
static std::unordered_map<std::string, Oop> internedSymbols;

Oop Oop::internSymbol(const std::string &string)
{
    auto it = internedSymbols.find(string);
    if(it != internedSymbols.end())
        return it->second;

    auto object = staticNewInstance<Symbol> (string.size());
    if(!string.empty())
        memcpy(object->variableData(), string.data(), string.size());

    auto oop = Oop::fromObjectPtr(object);
    internedSymbols.insert(std::make_pair(string, oop));
    return oop;
}

static NativeClassRegistration<Symbol> symbolClassRegistration;

MethodBindings Symbol::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("printString", &SelfType::printString),
    };
}

std::string Symbol::printString() const
{
    std::string out;
    out.reserve(size() + 3);
    out.push_back('#');
    out.push_back('\'');
    for(auto c : *this)
    {
        if(c == '\'')
            out.push_back('\'');
        out.push_back(c);
    }

    out.push_back('\'');
    return out;
}


} // End of namespace nyast
