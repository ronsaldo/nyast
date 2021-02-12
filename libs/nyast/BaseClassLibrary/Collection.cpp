#include "nyast/BaseClassLibrary/Collection.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<Collection> collectionClassRegistration;

MethodBindings Collection::__instanceMethods__()
{
    return MethodBindings{
    };
}

} // End of namespace nyast
