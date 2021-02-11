#include "nyast/BaseClassLibrary/NativeMethod.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"

namespace nyast
{
static NativeClassRegistration<NativeMethod> nativeMethodClassRegistration;

MethodLookupResult NativeMethod::asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const
{
    return MethodLookupResult{entryPoint, trampolineSet.nativeMethodDispatchTrampoline};
}

} // End of namespace nyast
