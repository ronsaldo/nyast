#include "nyast/BaseClassLibrary/Symbol.hpp"
#include "nyast/BaseClassLibrary/Dictionary.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{
static RootOop internedSymbols;

Oop Oop::internSymbol(const std::string &string)
{
    if(internedSymbols.isNull())
        internedSymbols = staticOopNewInstance<Dictionary> ();

    auto stringOop = Oop::fromString(string);
    auto existent = internedSymbols->atOrNil(stringOop);
    if(existent.isNotNilOrNull())
        return existent;
    
    auto object = staticNewInstance<Symbol> (string.size());
    if(!string.empty())
        memcpy(object->variableData(), string.data(), string.size());

    auto oop = Oop::fromObjectPtr(object);
    internedSymbols->atPut(stringOop, oop);
    return oop;
}

static NativeClassRegistration<Symbol> symbolClassRegistration;

MethodCategories Symbol::__instanceMethods__()
{
    return MethodCategories{
        {"printing", {
            makeMethodBinding("printString", &SelfType::printString),
        }},
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
