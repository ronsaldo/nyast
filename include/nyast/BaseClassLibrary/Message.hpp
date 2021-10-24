#ifndef NYAST_BASE_CLASS_LIBRARY_MESSAGE_HPP
#define NYAST_BASE_CLASS_LIBRARY_MESSAGE_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Message : Subclass<Object, Message>
{
    static constexpr char const __className__[] = "Message";
    static SlotDefinitions __slots__();

    std::string printString() const;

    MemberOop selector;
    MemberOop args;
    MemberOop lookupClass;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_MESSAGE_HPP
