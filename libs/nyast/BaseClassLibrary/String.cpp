#include "nyast/BaseClassLibrary/String.hpp"

namespace nyast
{

Oop Oop::fromString(const std::string &string)
{
    auto object = newInstance<String> (string.size());
    if(!string.empty())
        memcpy(object->variableData(), string.data(), string.size());
    return Oop::fromObjectPtr(object);
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
