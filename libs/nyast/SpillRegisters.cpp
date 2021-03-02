#include "nyast/Oop.hpp"

namespace nyast
{

NYAST_CORE_EXPORT StackSpilledRegisters spillRegistersOntoStack(
    uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3,
    uintptr_t arg4, uintptr_t arg5, uintptr_t arg6, uintptr_t arg7,
    uintptr_t arg8, uintptr_t arg9, uintptr_t arg10, uintptr_t arg11,
    uintptr_t arg12, uintptr_t arg13, uintptr_t arg14, uintptr_t arg15
)
{
    return StackSpilledRegisters{
        {
            arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
            arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15
        },
        0
    };
}

} // End of namespace nyast
