#include "nyast/BaseClassLibrary/NativeClassRegistry.hpp"
#include <iostream>

namespace nyast
{

void AbstractNativeClassRegistration::addClassRegistration(AbstractNativeClassRegistration *registration)
{
    NativeClassRegistry::get().addClassRegistration(registration);
}

void AbstractNativeClassRegistration::removeClassRegistration(AbstractNativeClassRegistration *registration)
{
    NativeClassRegistry::get().removeClassRegistration(registration);
}

std::once_flag NativeClassRegistry::uniqueInstanceCreationFlag;
std::shared_ptr<NativeClassRegistry> NativeClassRegistry::uniqueInstance;

NativeClassRegistry &NativeClassRegistry::get()
{
    std::call_once(uniqueInstanceCreationFlag, []{
        uniqueInstance = std::make_shared<NativeClassRegistry> ();
    });
    return *uniqueInstance;
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
