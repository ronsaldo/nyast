#ifndef NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRATION_HPP
#define NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRATION_HPP

#include "../Oop.hpp"
#include <string>

namespace nyast
{

struct AbstractNativeClassRegistration
{
    static void addClassRegistration(AbstractNativeClassRegistration *registration);
    static void removeClassRegistration(AbstractNativeClassRegistration *registration);

    virtual std::string getClassName() const = 0;
    virtual Oop getClass() const = 0;
};

template<typename NCT>
struct NativeClassRegistration : AbstractNativeClassRegistration
{
    typedef NCT NativeClassType;

    NativeClassRegistration()
    {
        addClassRegistration(this);
    }

    ~NativeClassRegistration()
    {
        removeClassRegistration(this);
    }

    virtual std::string getClassName() const override
    {
        return NativeClassType::__className__;
    }

    virtual Oop getClass() const override
    {
        return NativeClassType::__class__();
    }
};

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_NATIVE_CLASS_REGISTRATION_HPP
