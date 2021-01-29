#include "nyast/BaseClassLibrary/Association.hpp"

namespace nyast
{

Oop Association::getKey() const
{
    return key;
}

Oop Association::evaluateValue() const
{
    return value;
}

} // End of namespace nyast
