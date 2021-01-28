#ifndef NYAST_OBJECT_HPP
#define NYAST_OBJECT_HPP

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <vector>
#include <functional>
#include <ostream>

namespace nyast
{
typedef uintptr_t AbiOop;
struct Oop;

struct NyastObjectVTable;
struct NyastObject
{
    NyastObject() {}

    const NyastObjectVTable *__vtable;

    Oop self() const;
};

struct NyastObjectDispatcher;

typedef std::vector<uint8_t> ByteArrayData;

template<typename T, typename Enable=void>
struct CppToOop;

template<typename T, typename Enable=void>
struct OopToCpp;

/**
 * I am the result for a method lookup,
 */
struct MethodLookupResult
{
    void *methodObjectOrEntryPoint;
    void *dispatchTrampoline;

    template<typename ResultType, typename...Args>
    ResultType apply(Args... args)
    {
        return reinterpret_cast<ResultType (*) (void*, Args...)> (dispatchTrampoline) (methodObjectOrEntryPoint, args...);
    }
};

/**
 * I am a set of method dispatch trampolines that are used for adjusting the method arguments in a message send site.
 */
struct MessageDispatchTrampolineSet
{
    void *nativeMethodDispatchTrampoline;
    void *objectMethodDispatchTrampoline;

    template<typename ResultType, typename...Args>
    static MessageDispatchTrampolineSet forSignature();
};

/**
 * I am the structure layout for an inline cache.
 */
struct InlineCache
{
};

template<typename T>
struct OopPointerSizeDependentImplementation;

template<>
struct OopPointerSizeDependentImplementation<uint32_t>
{
    typedef uint32_t UnsignedValueType;
    typedef int32_t SignedValueType;

    static constexpr uint32_t OopBits = 32;
    static constexpr uint32_t TagBits = 2;
    static constexpr uint32_t TagMask = (1u<<TagBits) - 1u;

    static constexpr uint32_t PointerTagMask = TagMask;
    static constexpr uint32_t PointerTagValue = 0;
    static constexpr uint32_t PointerTagShift = 0;

    static constexpr uint32_t SmallIntegerTagMask = 1;
    static constexpr uint32_t SmallIntegerTagValue = 1;
    static constexpr uint32_t SmallIntegerTagShift = 1;

    static constexpr uint32_t CharacterTagMask = TagMask;
    static constexpr uint32_t CharacterTagValue = 2;
    static constexpr uint32_t CharacterTagShift = TagBits;

    static constexpr size_t ImmediateClassTableSize = 1<<TagBits;
    static NyastObject *ImmediateClassTable[ImmediateClassTableSize];
    static const NyastObjectVTable *ImmediateVTableTable[ImmediateClassTableSize];

    bool isSmallFloat() const
    {
        return false;
    }

    double decodeSmallFloat() const
    {
        return 0.0;
    }
};

template<>
struct OopPointerSizeDependentImplementation<uint64_t>
{
    typedef uint32_t UnsignedValueType;
    typedef int32_t SignedValueType;

    static constexpr uint64_t OopBits = 64;
    static constexpr uint64_t TagBits = 3;
    static constexpr uint64_t TagMask = (1u<<TagBits) - 1u;

    static constexpr uint64_t PointerTagMask = TagMask;
    static constexpr uint64_t PointerTagValue = 0;
    static constexpr uint64_t PointerTagShift = 0;

    static constexpr uint64_t SmallIntegerTagMask = TagMask;
    static constexpr uint64_t SmallIntegerTagValue = 1;
    static constexpr uint64_t SmallIntegerTagShift = TagBits;

    static constexpr uint64_t CharacterTagMask = TagMask;
    static constexpr uint64_t CharacterTagValue = 2;
    static constexpr uint64_t CharacterTagShift = TagBits;

    static constexpr uint64_t SmallFloatTagMask = TagMask;
    static constexpr uint64_t SmallFloatTagValue = 4;
    static constexpr uint64_t SmallFloatTagShift = TagBits;

    static constexpr size_t ImmediateClassTableSize = 1<<TagBits;
    static NyastObject *ImmediateClassTable[ImmediateClassTableSize];
    static const NyastObjectVTable *ImmediateVTableTable[ImmediateClassTableSize];

    bool isSmallFloat() const
    {
        return (value & SmallFloatTagMask) == SmallFloatTagValue;
    }

    double decodeSmallFloat() const
    {
        return 0.0;
    }

    uint64_t value;
};

/**
 * I am an ordinary object pointer.
 */
struct Oop : OopPointerSizeDependentImplementation<AbiOop>
{
    static constexpr intptr_t SmallIntegerUsableBits = OopBits - SmallIntegerTagShift;
    static constexpr intptr_t SmallIntegerMinValue = -( ((intptr_t)1)<<(SmallIntegerUsableBits - 1) ) ;
    static constexpr intptr_t SmallIntegerMaxValue = +( ((intptr_t)1)<<(SmallIntegerUsableBits - 1) ) - 1;

    Oop();
    explicit Oop(uintptr_t cvalue) : OopPointerSizeDependentImplementation{cvalue} {}

    static Oop nil();
    static Oop trueValue();
    static Oop falseValue();

    static Oop fromObjectPtr(const NyastObject *ptr)
    {
        return Oop{reinterpret_cast<uintptr_t> (ptr)};
    }

    static Oop fromChar32(char32_t value)
    {
        return Oop{(uintptr_t(value) << CharacterTagShift) | CharacterTagValue};
    }

    static Oop fromBoolean8(bool value)
    {
        return value ? trueValue() : falseValue();
    }

    static Oop fromInt32(int32_t value)
    {
        if constexpr(sizeof(int32_t) < sizeof(uintptr_t))
            return Oop{(value << SmallIntegerTagShift) | SmallIntegerTagValue};
        else
        {
            if(SmallIntegerMinValue <= value && value <= SmallIntegerMaxValue)
                return Oop{(uint32_t(value) << SmallIntegerTagShift) | SmallIntegerTagValue};
            else
                return fromInt64(value);
        }
    }

    static Oop fromUInt32(uint32_t value)
    {
        if constexpr(sizeof(int32_t) < sizeof(uintptr_t))
            return Oop{(value << SmallIntegerTagShift) | SmallIntegerTagValue};
        else
        {
            if(value <= uint32_t(SmallIntegerMaxValue))
                return Oop{(value << SmallIntegerTagShift) | SmallIntegerTagValue};
            else
                return fromUInt64(value);
        }
    }

    static Oop fromUIntPtr(uintptr_t value)
    {
        if constexpr(sizeof(uint32_t) == sizeof(uintptr_t))
            return fromUInt32(uint32_t(value));
        else
            return fromUInt64(value);
    }

    static Oop fromIntPtr(intptr_t value)
    {
        if constexpr(sizeof(int32_t) == sizeof(intptr_t))
            return fromInt32(int32_t(value));
        else
            return fromInt64(value);
    }

    static Oop fromSize(size_t value)
    {
        return fromUIntPtr(value);
    }

    static Oop fromInt64(int64_t value);
    static Oop fromUInt64(uint64_t value);
    static Oop fromFloat64(double value);
    static Oop fromOopList(const std::vector<Oop> &value);
    static Oop fromByteArray(const ByteArrayData &value);

    static Oop fromString(const std::string &string);
    static Oop internSymbol(const std::string &string);

    bool isNotNil() const
    {
        return value != nil().value;
    }

    bool isNotNilOrNull() const
    {
        return value != 0 && value != nil().value;
    }

    bool isNil() const
    {
        return value == nil().value;
    }

    bool isPointer() const
    {
        return (value & PointerTagMask) == PointerTagValue;
    }

    bool isImmediate() const
    {
        return !isPointer();
    }

    bool isSmallInteger() const
    {
        return (value & SmallIntegerTagMask) == SmallIntegerTagValue;
    }

    bool isCharacter() const
    {
        return (value & CharacterTagMask) == CharacterTagValue;
    }

    NyastObject *asObjectPtr()
    {
        return reinterpret_cast<NyastObject*> (value);
    }

    NyastObjectDispatcher *operator->() const
    {
        return reinterpret_cast<NyastObjectDispatcher*> (value);
    }

    template<typename T>
    auto as()
    {
        return OopToCpp<T>() (*this);
    }

    intptr_t decodeSmallInteger() const
    {
        assert(isSmallInteger());
        return static_cast<intptr_t> (value) >> SmallIntegerTagShift;
    }

    char32_t decodeCharacter() const
    {
        assert(isCharacter());
        return char32_t(value >> CharacterTagShift);
    }

    const NyastObjectVTable *getVTable() const;
    Oop getClass() const;

    template<typename ResultType, typename... Args>
    ResultType typedPerformInSuperclass(InlineCache *inlineCache, Oop clazz, Oop typedSelector, Args... args);

    template<typename ResultType, typename... Args>
    ResultType performInSuperclass(InlineCache *inlineCache, Oop clazz, Oop selector, Args... args);

    template<typename ResultType, typename... Args>
    ResultType typedPerform(InlineCache *inlineCache, Oop typedSelector, Args... args)
    {
        return typedPerformInSuperclass<ResultType> (inlineCache, getClass(), typedSelector, args...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(InlineCache *inlineCache, Oop selector, Args... args)
    {
        return performInSuperclass<ResultType> (inlineCache, getClass(), selector, args...);
    }

    template<typename ResultType, typename... Args>
    ResultType typedPerform(InlineCache *inlineCache, const std::string &typedSelector, Args... args)
    {
        return typedPerform<ResultType> (inlineCache, internSymbol(typedSelector), args...);
    }

    template<typename ResultType, typename... Args>
    ResultType typedPerform(Oop typedSelector, Args... args)
    {
        return typedPerform<ResultType> (nullptr, typedSelector, args...);
    }

    template<typename ResultType, typename... Args>
    ResultType typedPerform(const std::string &typedSelector, Args... args)
    {
        return typedPerform<ResultType> (nullptr, typedSelector, args...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(InlineCache *inlineCache, const std::string &selector, Args... args)
    {
        return perform<ResultType> (inlineCache, internSymbol(selector), args...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(Oop selector, Args... args)
    {
        return perform<ResultType> (nullptr, selector, args...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(const std::string &selector, Args... args)
    {
        return perform<ResultType> (nullptr, selector, args...);
    }

    bool operator==(const Oop &o) const
    {
        return value == o.value;
    }

    bool operator!=(const Oop &o) const
    {
        return value != o.value;
    }

    bool operator<(const Oop &o) const
    {
        return value < o.value;
    }

    bool operator<=(const Oop &o) const
    {
        return value <= o.value;
    }

    bool operator>(const Oop &o) const
    {
        return value > o.value;
    }

    bool operator>=(const Oop &o) const
    {
        return value >= o.value;
    }

    Oop performInSuperclassWithArguments(InlineCache *inlineCache, Oop clazz, Oop selector, const std::vector<Oop> &arguments);

    Oop performInSuperclassWithArguments(Oop clazz, Oop selector, const std::vector<Oop> &arguments)
    {
        return performInSuperclassWithArguments(nullptr, clazz, selector, arguments);
    }

    Oop performWithArguments(InlineCache *inlineCache, Oop selector, const std::vector<Oop> &arguments)
    {
        return performInSuperclassWithArguments(inlineCache, getClass(), selector, arguments);
    }

    Oop performWithArguments(Oop selector, const std::vector<Oop> &arguments)
    {
        return performWithArguments(nullptr, selector, arguments);
    }
};

inline Oop NyastObject::self() const
{
    return Oop::fromObjectPtr(this);
}

typedef std::vector<Oop> OopList;

template<typename T>
struct TypedOop : Oop
{
    T *operator*()
    {
        return as<T> ();
    }
};

/**
 * I am a definition of the nyast object vtable.
 */
struct NyastObjectVTable
{
    // Construction / finalization
    void (*basicInitialize) (AbiOop self);
    void (*initialize) (AbiOop self);
    void (*finalize) (AbiOop self);

    // Reflection core.
    Oop (*getClass) (AbiOop self);
    Oop (*lookupSelector)(AbiOop self, Oop selector);
    Oop (*runWithIn) (AbiOop self, Oop selector, const OopList &marshalledArguments, Oop receiver);
    MethodLookupResult (*asMethodLookupResult)(AbiOop self, MessageDispatchTrampolineSet trampolineSet);

    // Basic operations
    size_t (*identityHash)(AbiOop self);
    bool (*identityEquals)(AbiOop self, Oop other);
    size_t (*hash)(AbiOop self);
    bool (*equals)(AbiOop self, Oop other);

    // Common collection methods.
    size_t (*getBasicSize)(AbiOop self);
    Oop (*basicAt)(AbiOop self, size_t key);
    Oop (*basicAtPut)(AbiOop self, size_t key, Oop value);

    size_t (*getSize)(AbiOop self);
    Oop (*at)(AbiOop self, Oop key);
    Oop (*atPut)(AbiOop self, Oop key, Oop value);

    Oop (*atOrNil)(AbiOop self, Oop key);
    Oop (*scanFor)(AbiOop self, Oop key);
    void (*grow)(AbiOop self);
    Oop (*add)(AbiOop self, Oop value);

    // Association
    Oop (*getKey)(AbiOop self);

    // Blocks
    Oop (*evaluateValue)(AbiOop self);

    // Testing methods
    bool (*isArray)(AbiOop self);
    bool (*isAssociation)(AbiOop self);
    bool (*isBehavior)(AbiOop self);
    bool (*isBlock)(AbiOop self);
    bool (*isCharacter)(AbiOop self);
    bool (*isDictionary)(AbiOop self);
    bool (*isFloat)(AbiOop self);
    bool (*isFraction)(AbiOop self);
    bool (*isInteger)(AbiOop self);
    bool (*isInterval)(AbiOop self);
    bool (*isNumber)(AbiOop self);
    bool (*isString)(AbiOop self);
    bool (*isSymbol)(AbiOop self);

    // Basic conversions
    std::string (*asString)(AbiOop self);
    std::string (*printString)(AbiOop self);
    bool (*asBoolean8)(AbiOop self);
    uint32_t (*asUInt32)(AbiOop self);
    int32_t (*asInt32)(AbiOop self);
    uint64_t (*asUInt64)(AbiOop self);
    int64_t (*asInt64)(AbiOop self);
    char32_t (*asChar32)(AbiOop self);
    double (*asFloat64)(AbiOop self);
    std::string (*asStdString)(AbiOop self);
    std::vector<Oop> (*asOopList)(AbiOop self);
    ByteArrayData (*asByteArrayData)(AbiOop self);
};

struct NyastObjectDispatcher
{
    Oop self() const
    {
        return Oop::fromObjectPtr(reinterpret_cast<const NyastObject*> (this));
    }

    AbiOop abiSelf() const
    {
        return self().value;
    }

    const NyastObjectVTable *__vtable() const
    {
        return self().getVTable();
    }

    // Construction / finalization
    void basicInitialize()
    {
        __vtable()->basicInitialize(abiSelf());
    }

    void initialize()
    {
        __vtable()->initialize(abiSelf());
    }

    void finalize()
    {
        __vtable()->finalize(abiSelf());
    }

    // Reflection core.
    Oop getClass() const
    {
        return __vtable()->getClass(abiSelf());
    }

    std::string getClassName() const
    {
        return getClass()->asString();
    }

    Oop lookupSelector(Oop selector) const
    {
        return __vtable()->lookupSelector(abiSelf(), selector);
    }

    Oop runWithIn(Oop selector, const OopList &marshalledArguments, Oop receiver)
    {
        return __vtable()->runWithIn(abiSelf(), selector, marshalledArguments, receiver);
    }

    MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const
    {
        return __vtable()->asMethodLookupResult(abiSelf(), trampolineSet);
    }

    // Basic operations.
    std::size_t identityHash() const
    {
        return __vtable()->identityHash(abiSelf());
    }

    bool identityEquals(Oop other) const
    {
        return __vtable()->identityEquals(abiSelf(), other);
    }

    std::size_t hash() const
    {
        return __vtable()->hash(abiSelf());
    }

    bool equals(Oop other) const
    {
        return __vtable()->equals(abiSelf(), other);
    }

    bool asBoolean8() const
    {
        return __vtable()->asBoolean8(abiSelf());
    }

    // Common collection methods.
    size_t getBasicSize() const
    {
        return __vtable()->getBasicSize(abiSelf());
    }

    Oop basicAt(size_t key) const
    {
        return __vtable()->basicAt(abiSelf(), key);
    }

    Oop basicAtPut(size_t key, Oop value)
    {
        return __vtable()->basicAtPut(abiSelf(), key, value);
    }

    size_t getSize() const
    {
        return __vtable()->getSize(abiSelf());
    }

    Oop at(Oop key) const
    {
        return __vtable()->at(abiSelf(), key);
    }

    Oop atPut(Oop key, Oop value)
    {
        return __vtable()->atPut(abiSelf(), key, value);
    }

    Oop atOrNil(Oop key) const
    {
        return __vtable()->atOrNil(abiSelf(), key);
    }

    Oop scanFor(Oop key) const
    {
        return __vtable()->scanFor(abiSelf(), key);
    }

    void grow()
    {
        return __vtable()->grow(abiSelf());
    }

    Oop add(Oop value)
    {
        return __vtable()->add(abiSelf(), value);
    }

    // Association
    Oop getKey() const
    {
        return __vtable()->getKey(abiSelf());
    }

    Oop evaluateValue() const
    {
        return __vtable()->evaluateValue(abiSelf());
    }

    // Testing methods.
    bool isArray() const
    {
        return __vtable()->isArray(abiSelf());
    }

    bool isAssociation() const
    {
        return __vtable()->isAssociation(abiSelf());
    }

    bool isBehavior() const
    {
        return __vtable()->isBehavior(abiSelf());
    }

    bool isBlock() const
    {
        return __vtable()->isBlock(abiSelf());
    }

    bool isCharacter() const
    {
        return __vtable()->isCharacter(abiSelf());
    }

    bool isDictionary() const
    {
        return __vtable()->isDictionary(abiSelf());
    }

    bool isFloat() const
    {
        return __vtable()->isFloat(abiSelf());
    }

    bool isFraction() const
    {
        return __vtable()->isFraction(abiSelf());
    }

    bool isInteger() const
    {
        return __vtable()->isInteger(abiSelf());
    }

    bool isInterval() const
    {
        return __vtable()->isInterval(abiSelf());
    }

    bool isNumber() const
    {
        return __vtable()->isNumber(abiSelf());
    }

    bool isString() const
    {
        return __vtable()->isString(abiSelf());
    }

    bool isSymbol() const
    {
        return __vtable()->isSymbol(abiSelf());
    }

    // Basic conversions.
    uint32_t asUInt32() const
    {
        return __vtable()->asUInt32(abiSelf());
    }

    int32_t asInt32() const
    {
        return __vtable()->asInt32(abiSelf());
    }

    uint64_t asUInt64() const
    {
        return __vtable()->asUInt64(abiSelf());
    }

    int64_t asInt64() const
    {
        return __vtable()->asInt64(abiSelf());
    }

    char32_t asChar32() const
    {
        return __vtable()->asChar32(abiSelf());
    }

    double asFloat64() const
    {
        return __vtable()->asFloat64(abiSelf());
    }

    std::string asString() const
    {
        return __vtable()->asString(abiSelf());
    }

    std::string printString() const
    {
        return __vtable()->printString(abiSelf());
    }

    std::string asStdString() const
    {
        return __vtable()->asStdString(abiSelf());
    }

    std::vector<Oop> asOopList() const
    {
        return __vtable()->asOopList(abiSelf());
    }

    ByteArrayData asByteArrayData() const
    {
        return __vtable()->asByteArrayData(abiSelf());
    }
};

inline std::ostream &operator<<(std::ostream &out, const Oop &self)
{
    out << self->asString();
    return out;
}

inline const NyastObjectVTable *Oop::getVTable() const
{
    if(isPointer())
        return reinterpret_cast<NyastObject*> (value)->__vtable;

    return ImmediateVTableTable[value & TagMask];
}

inline Oop Oop::getClass() const
{
    return getVTable()->getClass(value);
}

inline std::ostream &operator<<(std::ostream &out, const OopList &list)
{
    out << "#(";
    bool isFirst = true;
    for(auto &el : list)
    {
        if(isFirst)
            isFirst = false;
        else
            out << ' ';
        out << el;
    }

    out << ')';
    return out;
}

template<typename T>
struct CppToOop<const T&> : CppToOop<T> {};

template<>
struct CppToOop<Oop>
{
    Oop operator()(Oop v)
    {
        return v;
    }
};

template<>
struct CppToOop<std::nullptr_t>
{
    Oop operator()(std::nullptr_t)
    {
        return Oop::nil();
    }
};

template<>
struct CppToOop<bool>
{
    Oop operator()(bool v)
    {
        return Oop::fromBoolean8(v);
    }
};

template<>
struct CppToOop<char32_t>
{
    Oop operator()(char32_t v)
    {
        return Oop::fromChar32(v);
    }
};

template<>
struct CppToOop<char16_t> : CppToOop<char32_t> {};

template<>
struct CppToOop<char> : CppToOop<char16_t> {};

template<>
struct CppToOop<uint32_t>
{
    Oop operator()(uint32_t v)
    {
        return Oop::fromUInt32(v);
    }
};

template<>
struct CppToOop<int32_t>
{
    Oop operator()(int32_t v)
    {
        return Oop::fromInt32(v);
    }
};

template<>
struct CppToOop<uint64_t>
{
    Oop operator()(uint64_t v)
    {
        return Oop::fromUInt64(v);
    }
};

template<>
struct CppToOop<int64_t>
{
    Oop operator()(int64_t v)
    {
        return Oop::fromInt64(v);
    }
};

template<>
struct CppToOop<uint16_t> : CppToOop<uint32_t> {};

template<>
struct CppToOop<uint8_t> : CppToOop<uint16_t> {};

template<>
struct CppToOop<int16_t> : CppToOop<int32_t> {};

template<>
struct CppToOop<int8_t> : CppToOop<int16_t> {};

template<>
struct CppToOop<double>
{
    Oop operator()(double v)
    {
        return Oop::fromFloat64(v);
    }
};

template<>
struct CppToOop<float> : CppToOop<double> {};

template<>
struct CppToOop<std::string>
{
    Oop operator()(const std::string &v)
    {
        return Oop::fromString(v);
    }
};

template<typename T>
struct CppToOop<T*, typename std::enable_if<std::is_base_of<NyastObject, T>::value>::type>
{
    Oop operator()(T* v)
    {
        return Oop::fromObjectPtr(v);
    }
};

template<typename T>
struct OopToCpp<T*, typename std::enable_if<std::is_base_of<NyastObject, T>::value>::type>
{
    T *operator()(Oop v)
    {
        return static_cast<T*> (v.asObjectPtr());
    }
};

template<>
struct CppToOop<const char*>
{
    Oop operator()(const char *v)
    {
        return Oop::fromString(v);
    }
};

template<>
struct CppToOop<OopList>
{
    Oop operator()(const OopList &v)
    {
        return Oop::fromOopList(v);
    }
};

template<>
struct OopToCpp<Oop>
{
    Oop operator()(Oop v)
    {
        return v;
    }
};

template<>
struct OopToCpp<bool>
{
    uint32_t operator()(Oop v)
    {
        return v->asBoolean8();
    }
};

template<>
struct OopToCpp<char32_t>
{
    char32_t operator()(Oop v)
    {
        return v->asChar32();
    }
};

template<>
struct OopToCpp<char16_t>
{
    char16_t operator()(Oop v)
    {
        return char16_t(v->asChar32());
    }
};

template<>
struct OopToCpp<char>
{
    char operator()(Oop v)
    {
        return char(v->asChar32());
    }
};

template<>
struct OopToCpp<uint8_t>
{
    uint8_t operator()(Oop v)
    {
        return uint8_t(v->asUInt32());
    }
};

template<>
struct OopToCpp<uint16_t>
{
    uint16_t operator()(Oop v)
    {
        return uint16_t(v->asUInt32());
    }
};

template<>
struct OopToCpp<uint32_t>
{
    uint32_t operator()(Oop v)
    {
        return v->asUInt32();
    }
};

template<>
struct OopToCpp<uint64_t>
{
    uint64_t operator()(Oop v)
    {
        return v->asUInt64();
    }
};

template<>
struct OopToCpp<int8_t>
{
    int8_t operator()(Oop v)
    {
        return int8_t(v->asInt32());
    }
};

template<>
struct OopToCpp<int16_t>
{
    int16_t operator()(Oop v)
    {
        return int16_t(v->asInt32());
    }
};

template<>
struct OopToCpp<int32_t>
{
    int32_t operator()(Oop v)
    {
        return v->asInt32();
    }
};

template<>
struct OopToCpp<int64_t>
{
    int64_t operator()(Oop v)
    {
        return v->asInt64();
    }
};

template<>
struct OopToCpp<double>
{
    double operator()(Oop v)
    {
        return v->asFloat64();
    }
};

template<>
struct OopToCpp<std::string>
{
    std::string operator()(Oop v)
    {
        return v->asStdString();
    }
};

template<>
struct OopToCpp<const std::string &> : OopToCpp<std::string> {};

template<>
struct OopToCpp<OopList>
{
    OopList operator()(Oop v)
    {
        return v->asOopList();
    }
};

template<>
struct OopToCpp<const OopList &> : OopToCpp<OopList> {};


template<>
struct OopToCpp<void>
{
    void operator()(Oop)
    {
        // Nothing is required here.
    }
};

template<typename RT, typename...Args>
struct NativeMethodDispatchTrampoline
{
    static RT value(void *methodFunctionPointer, Oop selector, Oop self, Args... args)
    {
        (void)selector;
        return reinterpret_cast<RT (*) (Oop, Args...)> (methodFunctionPointer) (self, args...);
    }
};

template<typename ResultType, typename...Args>
struct ObjectMethodDispatchTrampoline
{
    static ResultType value(void *methodObject, Oop selector, Oop self, Args... args)
    {
        auto methodOop = Oop::fromObjectPtr(reinterpret_cast<NyastObject*> (methodObject));
        OopList marshalledArguments{
            CppToOop<Args> ()(args)...
        };

        return methodOop->runWithIn(selector, marshalledArguments, self);
    }
};

template<typename ResultType, typename...Args>
inline MessageDispatchTrampolineSet MessageDispatchTrampolineSet::forSignature()
{
    return MessageDispatchTrampolineSet{
        reinterpret_cast<void*> (&NativeMethodDispatchTrampoline<ResultType, Args...>::value),
        reinterpret_cast<void*> (&ObjectMethodDispatchTrampoline<ResultType, Args...>::value),
    };
}

template<typename ResultType, typename... Args>
ResultType Oop::typedPerformInSuperclass(InlineCache *inlineCache, Oop clazz, Oop typedSelector, Args... args)
{
    (void)inlineCache;
    auto foundMethod = clazz->lookupSelector(typedSelector);
    if(foundMethod.isNil())
        foundMethod = clazz;
    auto lookupResult = foundMethod->asMethodLookupResult(MessageDispatchTrampolineSet::forSignature<ResultType, Args...> ());
    return lookupResult.template apply<ResultType> (typedSelector, *this, args...);
}

template<typename ResultType, typename... Args>
ResultType Oop::performInSuperclass(InlineCache *inlineCache, Oop clazz, Oop selector, Args... args)
{
    return OopToCpp<ResultType> ()(
        typedPerformInSuperclass<Oop> (inlineCache, clazz, selector,
            CppToOop<Args> ()(args)...
        )
    );
}

} // End of namespace nyast
#endif //NYAST_OBJECT_HPP
