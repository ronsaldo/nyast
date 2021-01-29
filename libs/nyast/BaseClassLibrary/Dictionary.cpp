#include "nyast/BaseClassLibrary/Dictionary.hpp"

namespace nyast
{

Oop Dictionary::atOrNil(Oop key) const
{
    auto elementIndexOop = self()->scanFor(key);
    if(!elementIndexOop.isSmallInteger())
        return Oop::nil();

    auto elementIndex = elementIndexOop.decodeSmallInteger();
    if(elementIndex == 0)
        return Oop::nil();

    return array->basicAt(elementIndex)
        ->evaluateValue();
}

} // End of namespace nyast
