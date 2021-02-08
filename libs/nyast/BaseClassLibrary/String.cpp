#include "nyast/BaseClassLibrary/String.hpp"
#include "nyast/BaseClassLibrary/Symbol.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

Oop Oop::fromString(const std::string &string)
{
    auto object = newInstance<String> (string.size());
    if(!string.empty())
        memcpy(object->variableData(), string.data(), string.size());
    return Oop::fromObjectPtr(object);
}

MethodBindings String::__instanceMethods__()
{
    return MethodBindings{
        makeMethodBinding("hash", &SelfType::hash),
        makeMethodBinding("=", &SelfType::equals),
        makeMethodBinding("asString", +[](Oop self) -> Oop {
            return self;
        }),
        makeMethodBinding("printString", &SelfType::printString),
    };
}

OopHash String::hash() const
{
    OopHash hash = /*String::__class__()->identityHash()*/13312 & OopHashModulo;
    for(auto c : *this)
    {
        hash = hashMultiply(hash + c);
    }

    return hash;
}

bool String::equals(Oop other) const
{
    // Fast case
    if(self() == other)
        return true;

    // Check the class.
    auto otherClass = other.getClass();
    if(otherClass != self().getClass() && otherClass != String::__class__() && otherClass != Symbol::__class__())
        return false;

    // Cast the other.
    auto otherString = other.as<SelfType> ();
    auto selfSize = size();
    if(selfSize != otherString->size())
        return false;

    return memcmp(variableData(), otherString->variableData(), selfSize) == 0;
}

std::string String::asStdString() const
{
    return std::string(begin(), end());
}

std::string String::asString() const
{
    return std::string(begin(), end());
}

std::string String::printString() const
{
    std::string out;
    out.reserve(size() + 2);
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
