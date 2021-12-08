#include "nyast/BaseClassLibrary/Association.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<Association> associationClassRegistration;

SlotDefinitions Association::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("value", &SelfType::value),
    };
}

MethodCategories Association::__classMethods__()
{
    return MethodCategories{
        {"constructing", {
            makeMethodBinding<Oop (Oop, Oop, Oop)> ("key:value:", +[](Oop clazz, Oop key, Oop value) {
                auto result = clazz->basicNewInstance();
                result.as<SelfType> ()->key = key;
                result.as<SelfType> ()->value = value;
                return result;
            }),
        }}
    };
}

MethodCategories Association::__instanceMethods__()
{
    return MethodCategories{
        {"testing", {
            makeMethodBinding("isAssociation", &SelfType::isAssociation),
        }},

        // Accessing
        {"accessing", {
            makeGetterMethodBinding("value", &SelfType::value),
            makeSetterMethodBinding("value:", &SelfType::value),
        }},

        // Printing.
        {"printing", {
            makeMethodBinding("asString", &SelfType::asString),
            makeMethodBinding("printString", &SelfType::printString),
        }}
    };
}

bool Association::isAssociation() const
{
    return true;
}

Oop Association::evaluateValue() const
{
    return value;
}

Oop Association::evaluateValueWithArg(Oop arg)
{
    value = arg;
    return self();
}

std::string Association::asString() const
{
    return self()->printString();
}

std::string Association::printString() const
{
    std::ostringstream out;
    out << key << "->" << value;
    return out.str();
}

} // End of namespace nyast
