#ifndef NYAST_BASE_CLASS_LIBRARY_CHARACTER_HPP
#define NYAST_BASE_CLASS_LIBRARY_CHARACTER_HPP

#pragma once

#include "Magnitude.hpp"

namespace nyast
{

struct NYAST_CORE_EXPORT Character : SubclassWithImmediateRepresentation<Magnitude, Character>
{
    static constexpr char const __className__[] = "Character";

    std::string asString() const;
    char32_t asChar32() const;
};

} // End of namespace namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_CHARACTER_HPP
