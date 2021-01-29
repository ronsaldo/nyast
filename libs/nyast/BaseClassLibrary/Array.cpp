#include "nyast/BaseClassLibrary/Array.hpp"

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

} // End of namespace nyast
