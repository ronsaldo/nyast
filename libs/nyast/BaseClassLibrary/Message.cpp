#include "nyast/BaseClassLibrary/Message.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<Message> messageClassRegistration;

std::string Message::printString() const
{
    std::ostringstream out;
    out << selector->printString();
    for(auto &arg : args->asOopList())
        out << " " << arg->printString();
    out << std::ends;
    return out.str();
}

} // End of namespace nyast
