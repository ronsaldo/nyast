#include "nyast/BaseClassLibrary/Collection.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<Collection> collectionClassRegistration;

MethodCategories Collection::__instanceMethods__()
{
    return MethodCategories{
    };
}

} // End of namespace nyast
