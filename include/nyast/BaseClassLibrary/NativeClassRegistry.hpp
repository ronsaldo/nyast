#ifndef NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRY_HPP
#define NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRY_HPP

#include "NativeClassRegistration.hpp"
#include <mutex>

namespace nyast
{

/**
 * I am a global registry for native classes.
 */
class NativeClassRegistry : public AbstractNativeClassRegistry
{
public:
    virtual void addClassRegistration(AbstractNativeClassRegistration *registration) override;
    virtual void removeClassRegistration(AbstractNativeClassRegistration *registration) override;

    virtual AbstractNativeClassRegistration *findClassRegistrationByName(const std::string &name) override;

    virtual void initializeRuntimeEnvironmentClasses() override;

private:
    std::mutex mutex;
    std::unordered_map<std::string, AbstractNativeClassRegistration*> nativeClassRegistrations;
};

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRY_HPP
