#include "nyast/BaseClassLibrary/Array.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<Array> arrayClassRegistration;

Oop Oop::fromOopList(const std::vector<Oop> &list)
{
    auto object = staticNewInstance<Array> (list.size());
    auto dest = object->variableData();
    for(size_t i = 0; i < list.size(); ++i)
        dest[i] = list[i];

    return Oop::fromObjectPtr(object);
}

MethodCategories Array::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding("basicSize", &SelfType::size),
            makeMethodBinding("basicAt:", &SelfType::basicAt),
            makeMethodBinding("basicAt:put:", &SelfType::basicAtPut),

            makeMethodBinding("size", &SelfType::getSize),
            makeMethodBinding("at:", &SelfType::at),
            makeMethodBinding("at:put:", &SelfType::atPut),
        }},

        {"testing", {
            makeMethodBinding("isArray", &SelfType::isArray),
        }},

        {"printing", {
            makeMethodBinding("asString", &SelfType::asString),
            makeMethodBinding("printString", &SelfType::printString),
        }},
    };
}

bool Array::isArray() const
{
    return true;
}

size_t Array::getSize() const
{
    return size();
}

Oop Array::at(Oop key) const
{
    return basicAt(key.as<size_t> ());
}

Oop Array::atPut(Oop key, Oop value)
{
    return basicAtPut(key.as<size_t> (), value);
}

OopList Array::asOopList() const
{
    return OopList(begin(), end());
}

std::string Array::asString() const
{
    return self()->printString();
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

    out << '}';
    return out.str();
}

} // End of namespace nyast
