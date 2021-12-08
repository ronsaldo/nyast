#include "nyast/BaseClassLibrary/Integer.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{

static NativeClassRegistration<Integer> integerClassRegistration;

MethodCategories Integer::__instanceMethods__()
{
    return MethodCategories{
        {"arithmetic", {
            makeMethodBinding<Oop (Oop)> ("factorial", +[](Oop self) {
                if(self.isSmallInteger() && self.decodeSmallInteger() <= 0 )
                    return Oop::fromInt32(1);
                auto n1 = self.perform<Oop> ("-", 1).perform<Oop> ("factorial");
                return n1.perform<Oop> ("*", self);
            }),
        }},
    };
}

} // End of namespace nyast
