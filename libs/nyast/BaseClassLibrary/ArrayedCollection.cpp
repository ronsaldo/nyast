#include "nyast/BaseClassLibrary/ArrayedCollection.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<ArrayedCollection> arrayedCollectionClassRegistration;

MethodBindings ArrayedCollection::__instanceMethods__()
{
    return MethodBindings{
    };
}

} // End of namespace nyast
