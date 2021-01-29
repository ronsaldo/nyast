#include "nyast/BaseClassLibrary/Character.hpp"

namespace nyast
{

std::string Character::asString() const
{
    std::string result;
    result.push_back(self().decodeCharacter());
    return result;
}

char32_t Character::asChar32() const
{
    return self().decodeCharacter();
}

} // End of namespace nyast
