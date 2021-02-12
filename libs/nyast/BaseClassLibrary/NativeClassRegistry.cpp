#include "nyast/BaseClassLibrary/NativeClassRegistry.hpp"
#include <iostream>

namespace nyast
{

static std::once_flag nativeClassRegistryUniqueInstanceCreationFlag;
static AbstractNativeClassRegistryPtr nativeClassRegistryUniqueInstance;

AbstractNativeClassRegistryPtr AbstractNativeClassRegistry::get()
{
    std::call_once(nativeClassRegistryUniqueInstanceCreationFlag, []{
        nativeClassRegistryUniqueInstance = std::make_shared<NativeClassRegistry> ();
    });
    return nativeClassRegistryUniqueInstance;
}

void NativeClassRegistry::addClassRegistration(AbstractNativeClassRegistration *registration)
{
    std::unique_lock l(mutex);
    nativeClassRegistrations[registration->getClassName()] = registration;
}

void NativeClassRegistry::removeClassRegistration(AbstractNativeClassRegistration *registration)
{
    std::unique_lock l(mutex);
    auto it = nativeClassRegistrations.find(registration->getClassName());
    if(it != nativeClassRegistrations.end())
        nativeClassRegistrations.erase(it);
}

AbstractNativeClassRegistration *NativeClassRegistry::findClassRegistrationByName(const std::string &name)
{
    std::unique_lock l(mutex);
    auto it = nativeClassRegistrations.find(name);
    return it != nativeClassRegistrations.end() ? it->second : nullptr;
}

} // End of namespace nyast
