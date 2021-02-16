#include "nyast/BaseClassLibrary/ArrayedCollection.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include <sstream>

namespace nyast
{

static NativeClassRegistration<ArrayedCollection> arrayedCollectionClassRegistration;

MethodCategories ArrayedCollection::__instanceMethods__()
{
    return MethodCategories{
    };
}

} // End of namespace nyast
