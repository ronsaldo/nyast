#include "nyast/BaseClassLibrary/Class.hpp"

namespace nyast
{

Oop Class::getClass() const
{
    return metaClass;
}

std::string Class::asString() const
{
    return name->asString();
}

} // End of namespace nyast
