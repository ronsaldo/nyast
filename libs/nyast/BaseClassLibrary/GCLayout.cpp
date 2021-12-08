#include "nyast/BaseClassLibrary/GCLayout.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<GCLayout> gcLayoutClassRegistration;

SlotDefinitions GCLayout::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("instanceSize", &SelfType::instanceSize),
        makeMemberSlot("variableDataLayout", &SelfType::variableDataLayout),
    };
}

GCLayout* GCLayout::forInstanceSize(size_t instanceSize, GCReferenceType variableDataLayout)
{
    auto wordCount = (instanceSize + LayoutsPerWord - 1) / LayoutsPerWord;
    auto layout = staticBasicNewInstance<GCLayout> (wordCount);
    layout->instanceSize = instanceSize;
    layout->variableDataLayout = variableDataLayout;
    return layout;
}

std::string GCLayout::asString() const
{
    return printString();
}

std::string GCLayout::printString() const
{
    std::ostringstream out;
    out << "GCLayout instanceSize: " << instanceSize << " fixedLayout: #(";
    fixedLayoutDo([&](size_t, GCReferenceType refType) {
        out << ' ' << GCReferenceTypeToString(refType);
    });

    out << " ) variableDataLayout: " << GCReferenceTypeToString(variableDataLayout);
    return out.str();
}

} // End of namespace nyast
