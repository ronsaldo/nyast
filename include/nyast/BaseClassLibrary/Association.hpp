#ifndef NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP
#define NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP

#pragma once

#include "LookupKey.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Association : Subclass<LookupKey, Association>
{
    static constexpr char const __className__[] = "Association";

    static MethodCategories __instanceMethods__();
    static MethodCategories __classMethods__();
    static SlotDefinitions __slots__();

    Association() = default;
    Association(Oop ckey, Oop cvalue)
        : DirectSuper(ckey), value(cvalue) {}

    bool isAssociation() const;

    Oop evaluateValue() const;
    Oop evaluateValueWithArg(Oop arg);

    std::string asString() const;
    std::string printString() const;

    MemberOop value;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_ASSOCIATION_HPP
