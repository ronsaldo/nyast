#include "nyast/BaseClassLibrary/ByteArray.hpp"
#include <sstream>

namespace nyast
{

Oop Oop::fromByteArray(const ByteArrayData &data)
{
    auto object = newInstance<ByteArray> (data.size());
    if(!data.empty())
        memcpy(object->variableData(), data.data(), data.size());
    return Oop::fromObjectPtr(object);
}

ByteArrayData ByteArray::asByteArrayData() const
{
    return ByteArrayData(begin(), end());
}

std::string ByteArray::asString() const
{
    return printString();
}

std::string ByteArray::printString() const
{
    std::ostringstream out;
    out << "#[";
    auto isFirst = true;
    for(auto &v : *this)
    {
        if(isFirst)
            isFirst = false;
        else
            out << ' ';

        out << int(v);
    }

    out << ']' << std::ends;
    return out.str();
}

} // End of namespace nyast