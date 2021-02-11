#include "nyast/BaseClassLibrary/Slot.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{
static NativeClassRegistration<Slot> slotClassRegistration;

Oop Slot::read(Oop )
{
    return subclassResponsibility();
}

Oop Slot::writeTo(Oop, Oop)
{
    return subclassResponsibility();
}

} // End of namespace nyast
