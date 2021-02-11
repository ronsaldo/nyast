#ifndef NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRY_HPP
#define NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRY_HPP

#include "NativeClassRegistration.hpp"
#include <mutex>
#include <memory>

namespace nyast
{

/**
 * I am a global registry for native classes.
 */
class NativeClassRegistry
{
public:
    static NativeClassRegistry &get();

    void addClassRegistration(AbstractNativeClassRegistration *registration);
    void removeClassRegistration(AbstractNativeClassRegistration *registration);

    AbstractNativeClassRegistration *findClassRegistrationByName(const std::string &name);

private:
    static std::once_flag uniqueInstanceCreationFlag;
    static std::shared_ptr<NativeClassRegistry> uniqueInstance;

    std::mutex mutex;
    std::unordered_map<std::string, AbstractNativeClassRegistration*> nativeClassRegistrations;
};

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRY_HPP
