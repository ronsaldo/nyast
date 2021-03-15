#ifndef NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRATION_HPP
#define NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRATION_HPP

#include "../Oop.hpp"
#include <string>
#include <memory>

namespace nyast
{

struct AbstractNativeClassRegistration;
struct AbstractNativeClassRegistry;
typedef std::shared_ptr<AbstractNativeClassRegistry> AbstractNativeClassRegistryPtr;

struct NYAST_CORE_EXPORT AbstractNativeClassRegistry
{
    virtual ~AbstractNativeClassRegistry() {};

    virtual void addClassRegistration(AbstractNativeClassRegistration *registration) = 0;
    virtual void removeClassRegistration(AbstractNativeClassRegistration *registration) = 0;

    virtual AbstractNativeClassRegistration *findClassRegistrationByName(const std::string &name) = 0;

    virtual void initializeRuntimeEnvironmentClasses() = 0;

    static AbstractNativeClassRegistryPtr get();
};

struct NYAST_CORE_EXPORT AbstractNativeClassRegistration
{
    virtual std::string getClassName() const = 0;
    virtual Oop getClass() const = 0;
};

template<typename NCT>
class NativeClassRegistration : AbstractNativeClassRegistration
{
public:
    typedef NCT NativeClassType;

    NativeClassRegistration()
    {
        registry = AbstractNativeClassRegistry::get();
        registry->addClassRegistration(this);
    }

    ~NativeClassRegistration()
    {
        registry->removeClassRegistration(this);
    }

    virtual std::string getClassName() const override
    {
        return NativeClassType::__className__;
    }

    virtual Oop getClass() const override
    {
        return NativeClassType::__class__();
    }

private:
    AbstractNativeClassRegistryPtr registry;
};

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRATION_HPP
