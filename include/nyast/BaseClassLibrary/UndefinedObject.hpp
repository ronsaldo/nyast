#ifndef NYAST_BASE_CLASS_LIBRARY_UNDEFINED_OBJECT_HPP
#define NYAST_BASE_CLASS_LIBRARY_UNDEFINED_OBJECT_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT UndefinedObject : Subclass<Object, UndefinedObject>
{
    static constexpr char const __className__[] = "UndefinedObject";

    static MethodCategories __instanceMethods__();
    
    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<UndefinedObject>::value();
    }

    Oop evaluateValue() const;

    std::string asString() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_UNDEFINED_OBJECT_HPP
