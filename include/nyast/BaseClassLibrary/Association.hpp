#ifndef NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP
#define NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP

#pragma once

#include "LookupKey.hpp"

namespace nyast
{

struct Association : Subclass<LookupKey, Association>
{
    static constexpr char const __className__[] = "Association";

    static MethodBindings __instanceMethods__();
    static MethodBindings __classMethods__();
    static SlotDefinitions __slots__();

    Association() = default;
    Association(Oop ckey, Oop cvalue)
        : DirectSuper(ckey), value(cvalue) {}

    bool isAssociation() const;

    Oop evaluateValue() const;

    std::string asString() const;
    std::string printString() const;

    MemberOop value;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP
