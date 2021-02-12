#ifndef NYAST_BASE_CLASS_LIBRARY_CPP_METHOD_BINDING_HPP
#define NYAST_BASE_CLASS_LIBRARY_CPP_METHOD_BINDING_HPP

#pragma once

#include "Object.hpp"

namespace nyast
{

/**
 * I am the base class for a cpp functional method.
 */
struct CppMethodBindingBase : Subclass<Object, CppMethodBindingBase>
{
    static constexpr char const __className__[] = "CppMethodBinding";

    CppMethodBindingBase(Oop cselector)
        : selector(cselector) {}

    MemberOop selector;
};

template<typename MethodSignature, typename FT>
struct CppMethodBinding;

template<typename T>
using CppTypeToOopType = Oop;

template<typename ResultType, typename... Args, typename FT>
struct CppMethodBinding<ResultType (Args...), FT> : CppMethodBindingBase
{
    typedef CppMethodBinding<ResultType (Args...), FT> SelfType;

    CppMethodBinding(Oop cselector, FT cfunctor)
        : CppMethodBindingBase(cselector), functor(cfunctor) {}

    static Oop trampoline(SelfType *methodBinding, Oop, typename CppToOopAbi<CppTypeToOopType<Args>>::type... args)
    {
        if constexpr(std::is_same<ResultType, void>::value)
        {
            methodBinding->functor(OopToCpp<Args> ()(
                OopAbiToCpp<CppTypeToOopType<Args>>()(args)
            )...);
            return Oop::nil();
        }
        else
        {
            return CppToOop<ResultType>() (
                methodBinding->functor(OopToCpp<Args> ()(
                    OopAbiToCpp<CppTypeToOopType<Args>>()(args)
                )...)
            );
        }
    }

    MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet) const
    {
        return MethodLookupResult{const_cast<SelfType*> (this), reinterpret_cast<void*> (&trampoline)};
    }

    FT functor;
};

template<typename MethodSignature, typename FT>
MethodBinding makeMethodBinding(const std::string &selector, FT &&functor)
{
    auto selectorOop = Oop::internSymbol(selector);
    auto methodBinding = staticBasicNewInstance<CppMethodBinding<MethodSignature, FT> > (0, selectorOop, functor);
    return MethodBinding{selectorOop, Oop::fromObjectPtr(methodBinding)};
}

template<typename SelfType, typename ResultType, typename... Args>
MethodBinding makeMethodBinding(const std::string &selector, ResultType (SelfType::*memberFunction)(Args...))
{
    return makeMethodBinding<ResultType (SelfType*, Args...)> (selector, [=](SelfType* self, Args&&... args){
        return (self->*memberFunction)(std::forward<Args> (args)...);
    });
}

template<typename SelfType, typename ResultType, typename... Args>
MethodBinding makeMethodBinding(const std::string &selector, ResultType (SelfType::*memberFunction)(Args...) const)
{
    return makeMethodBinding<ResultType (const SelfType*, Args...)> (selector, [=](const SelfType* self, Args&&... args){
        return (self->*memberFunction)(std::forward<Args> (args)...);
    });
}

template<typename ResultType, typename... Args>
MethodBinding makeMethodBinding(const std::string &selector, ResultType (*function)(Args...))
{
    return makeMethodBinding<ResultType (Args...)> (selector, [=](Args&&... args){
        return function(std::forward<Args> (args)...);
    });
}

template<typename MemberType, typename SelfType>
MethodBinding makeGetterMethodBinding(const std::string &selector, MemberType SelfType::*member )
{
    return makeMethodBinding<MemberType (const SelfType*)> (selector, [=](const SelfType *self){
        return self->*member;
    });
}

template<typename MemberType, typename SelfType>
MethodBinding makeSetterMethodBinding(const std::string &selector, MemberType SelfType::*member)
{
    typedef typename CppMemberToAccessorArgumentType<MemberType>::type ArgumentType;

    return makeMethodBinding<void (SelfType*, const ArgumentType &)> (selector, [=](SelfType *self, const ArgumentType &value){
        self->*member = value;
    });
}

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_CPP_METHOD_BINDING_HPP
