#ifndef NYAST_BASE_CLASS_LIBRARY_NATIVE_METHOD_HPP
#define NYAST_BASE_CLASS_LIBRARY_NATIVE_METHOD_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

/**
 * I am a direct native method.
 */
struct NativeMethod : Subclass<Object, NativeMethod>
{
    static constexpr char const __className__[] = "NativeMethod";

    MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const;

    MemberOop selector;
    void *entryPoint;
};

template<typename FT>
MethodBinding makeRawNativeMethodBinding(const std::string &selector, const FT &function)
{
    auto selectorOop = Oop::internSymbol(selector);
    auto nativeMethod = staticBasicNewInstance<NativeMethod> ();
    nativeMethod->selector = selectorOop;
    nativeMethod->entryPoint = reinterpret_cast<void*> (function);
    return MethodBinding{selectorOop, Oop::fromObjectPtr(nativeMethod)};
}

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_BEHAVIOR_HPP
