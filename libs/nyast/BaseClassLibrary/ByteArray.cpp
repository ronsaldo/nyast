#include "nyast/BaseClassLibrary/ByteArray.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<ByteArray> byteArrayClassRegistration;

Oop Oop::fromByteArray(const ByteArrayData &data)
{
    auto object = staticNewInstance<ByteArray> (data.size());
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

    out << ']';
    return out.str();
}

} // End of namespace nyast
