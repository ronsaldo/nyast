#include "nyast/BaseClassLibrary/NativeMethod.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"

namespace nyast
{
static NativeClassRegistration<NativeMethod> nativeMethodClassRegistration;

SlotDefinitions NativeMethod::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("selector", &SelfType::selector),
    };
}

MethodLookupResult NativeMethod::asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const
{
    return MethodLookupResult{entryPoint, trampolineSet.nativeMethodDispatchTrampoline};
}

} // End of namespace nyast
