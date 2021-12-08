#include "nyast/BaseClassLibrary/Message.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<Message> messageClassRegistration;

SlotDefinitions Message::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("selector", &SelfType::selector),
        makeMemberSlot("args", &SelfType::args),
        makeMemberSlot("lookupClass", &SelfType::lookupClass),
    };
}

std::string Message::printString() const
{
    std::ostringstream out;
    out << selector->printString();
    for(auto &arg : args->asOopList())
        out << " " << arg->printString();
    return out.str();
}

} // End of namespace nyast
