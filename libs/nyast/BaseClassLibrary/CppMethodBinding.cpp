#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<CppMethodBindingBase> cppMethodBindingBaseClassRegistration;

SlotDefinitions CppMethodBindingBase::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("selector", &SelfType::selector),
    };
}

MethodCategories CppMethodBindingBase::__instanceMethods__()
{
    return MethodCategories{
        {"printing", {
            makeMethodBinding("printString", &SelfType::printString),
        }},
    };
}

std::string CppMethodBindingBase::printString() const
{
    std::ostringstream out;
    out << "CppMethodBinding selector: " << selector->printString();
    return out.str();
}

std::string CppMethodBindingBase::asString() const
{
    return self()->printString();
}

} // End of namespace nyast
