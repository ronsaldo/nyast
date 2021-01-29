#include "nyast/BaseClassLibrary/Array.hpp"
#include <sstream>

namespace nyast
{

Oop Oop::fromOopList(const std::vector<Oop> &list)
{
    auto object = newInstance<Array> (list.size());
    auto dest = object->variableData();
    for(size_t i = 0; i < list.size(); ++i)
        dest[i] = list[i];

    return Oop::fromObjectPtr(object);
}

OopList Array::asOopList() const
{
    return OopList(begin(), end());
}

std::string Array::asString() const
{
    return printString();
}

std::string Array::printString() const
{
    std::ostringstream out;
    out << "{";
    auto isFirst = true;
    for(auto &v : *this)
    {
        if(isFirst)
            isFirst = false;
        else
            out << " . ";

        out << v;
    }

    out << '}' << std::ends;
    return out.str();
}

} // End of namespace nyast
