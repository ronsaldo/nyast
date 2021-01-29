#include "nyast/BaseClassLibrary/NativeMethod.hpp"

namespace nyast
{

MethodLookupResult NativeMethod::asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const
{
    return MethodLookupResult{entryPoint, trampolineSet.nativeMethodDispatchTrampoline};
}

} // End of namespace nyast
