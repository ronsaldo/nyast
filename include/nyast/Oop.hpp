#ifndef NYAST_OBJECT_HPP
#define NYAST_OBJECT_HPP

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <vector>
#include <functional>
#include <ostream>
#include "DllExport.hpp"

namespace nyast
{
typedef uintptr_t AbiOop;
struct NYAST_CORE_EXPORT Oop;

struct NyastObjectVTable;
struct NYAST_CORE_EXPORT NyastObject
{
    NyastObject() {}

    const NyastObjectVTable *__vtable = nullptr;

    uint32_t __variableDataSize;
    uint32_t __gcMarkingColor : 2;
    uint32_t __isReadOnly : 1;
    uint32_t __reservedBits : 29;

    Oop self() const;
};

struct NyastObjectDispatcher;

typedef std::vector<uint8_t> ByteArrayData;

template<typename T, typename Enable=void>
struct CppToOop;

template<typename T, typename Enable=void>
struct OopToCpp;

template<typename T>
struct CppToOopAbi
{
    typedef T type;

    T operator()(T value)
    {
        return value;
    }
};

template<typename T>
struct OopAbiToCpp
{
    typedef T type;

    T operator()(T value)
    {
        return value;
    }
};

/**
 * I describe the type of a reference for the Garbage Collector
 */
enum class GCReferenceType : uint8_t
{
    Value = 0,
    Strong = 1,
    Weak = 2,
};

static constexpr size_t GCReferenceTypeBitCount = 2;
static constexpr uintptr_t GCReferenceTypeBitMask = (1<<GCReferenceTypeBitCount) - 1;

NYAST_CORE_EXPORT const char * GCReferenceTypeToString(GCReferenceType referenceType);

/**
 * I am the result for a method lookup,
 */
struct NYAST_CORE_EXPORT MethodLookupResult
{
    void *methodObjectOrEntryPoint;
    void *dispatchTrampoline;

    template<typename ResultType, typename...Args>
    ResultType apply(Args... args)
    {
        return reinterpret_cast<ResultType (*) (void*, typename CppToOopAbi<Args>::type...)> (dispatchTrampoline) (methodObjectOrEntryPoint, CppToOopAbi<Args>()(args)...);
    }
};

/**
 * I am a set of method dispatch trampolines that are used for adjusting the method arguments in a message send site.
 */
struct NYAST_CORE_EXPORT MessageDispatchTrampolineSet
{
    void *nativeMethodDispatchTrampoline;
    void *objectMethodDispatchTrampoline;

    template<typename ResultType, typename...Args>
    static MessageDispatchTrampolineSet forSignature();
};

/**
 * I am the structure layout for an inline cache.
 */
struct NYAST_CORE_EXPORT InlineCache
{
};

typedef uintptr_t OopHash;
constexpr OopHash OopHashModulo = 0xFFFFFFF;
constexpr OopHash OopHashFactor = 1664525;

inline OopHash hashMultiply(OopHash hash)
{
    return (hash * OopHashFactor) & OopHashModulo;
}

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
    static const NyastObjectVTable *ImmediateVTableTable[ImmediateClassTableSize];

    static bool isFloatInSmallFloatRange(double)
    {
        return false;
    }

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

    static constexpr uint64_t SmallFloatExponentOffset = 896;

    static constexpr size_t ImmediateClassTableSize = 1<<TagBits;
    static const NyastObjectVTable *ImmediateVTableTable[ImmediateClassTableSize];

    static bool isFloatInSmallFloatRange(double v)
    {
        uint64_t bits;
        memcpy(&bits, &v, 8);

        // Rotate left.
        bits = (bits << 1) | (bits >> 63);

        auto subtractedExponent = SmallFloatExponentOffset << uint64_t(53);
        return bits >= subtractedExponent;
    }

    static uint64_t encodeSmallFloatValue(double v)
    {
        uint64_t bits;
        memcpy(&bits, &v, 8);

        // Rotate left.
        bits = (bits << 1) | (bits >> 63);

        // Subtract the exponent offset.
        auto subtractedExponent = SmallFloatExponentOffset << uint64_t(53);
        assert(bits >= subtractedExponent);
        bits -= subtractedExponent;

        // Add the tag
        return (bits << SmallFloatTagShift) | SmallFloatTagValue;
    }

    bool isSmallFloat() const
    {
        return (value & SmallFloatTagMask) == SmallFloatTagValue;
    }

    double decodeSmallFloat() const
    {
        // Shift out the tag.
        auto bits = value >> SmallFloatTagShift;

        // Add back the exponent offset.
        auto subtractedExponent = SmallFloatExponentOffset << uint64_t(53);
        bits += subtractedExponent;

        // Rotate right the sign back into its places.
        bits = (bits >> 1) | (bits & 1) << uint64_t(63);

        double floatValue;
        memcpy(&floatValue, &bits, 8);

        return floatValue;
    }

    uint64_t value;
};

/**
 * I am an ordinary object pointer.
 */
struct NYAST_CORE_EXPORT Oop : OopPointerSizeDependentImplementation<AbiOop>
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

    static Oop fromFloat64(double value)
    {
        if(isFloatInSmallFloatRange(value))
            return Oop{encodeSmallFloatValue(value)};
        else
            return makeBoxedFloat(value);
    }

    static Oop makeBoxedFloat(double value);

    static Oop fromInt64(int64_t value);
    static Oop fromUInt64(uint64_t value);
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

    bool isNull() const
    {
        return value == 0;
    }

    bool isNilOrNull() const
    {
        return value == 0 || value == nil().value;
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

    Oop asOop() const
    {
        return *this;
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
    ResultType typedPerformInSuperclass(InlineCache *inlineCache, Oop clazz, Oop typedSelector, Args&&... args);

    template<typename ResultType, typename... Args>
    ResultType performInSuperclass(InlineCache *inlineCache, Oop clazz, Oop selector, Args&&... args);

    template<typename ResultType, typename... Args>
    ResultType typedPerform(InlineCache *inlineCache, Oop typedSelector, Args&&... args)
    {
        return typedPerformInSuperclass<ResultType> (inlineCache, getClass(), typedSelector, std::forward<Args> (args)...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(InlineCache *inlineCache, Oop selector, Args&&... args)
    {
        return performInSuperclass<ResultType> (inlineCache, getClass(), selector, std::forward<Args> (args)...);
    }

    template<typename ResultType, typename... Args>
    ResultType typedPerform(InlineCache *inlineCache, const std::string &typedSelector, Args&&... args)
    {
        return typedPerform<ResultType> (inlineCache, internSymbol(typedSelector), std::forward<Args> (args)...);
    }

    template<typename ResultType, typename... Args>
    ResultType typedPerform(Oop typedSelector, Args&&... args)
    {
        return typedPerform<ResultType> (nullptr, typedSelector, std::forward<Args> (args)...);
    }

    template<typename ResultType, typename... Args>
    ResultType typedPerform(const std::string &typedSelector, Args&&... args)
    {
        return typedPerform<ResultType> (nullptr, typedSelector, std::forward<Args> (args)...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(InlineCache *inlineCache, const std::string &selector, Args&&... args)
    {
        return perform<ResultType> (inlineCache, internSymbol(selector), std::forward<Args> (args)...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(Oop selector, Args&&... args)
    {
        return perform<ResultType> (nullptr, selector, std::forward<Args> (args)...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(const std::string &selector, Args&&... args)
    {
        return perform<ResultType> (nullptr, selector, std::forward<Args> (args)...);
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

NYAST_CORE_EXPORT void registerGlobalOopRoots(size_t rootCount, Oop *roots);
NYAST_CORE_EXPORT void unregisterGlobalOopRoots(size_t rootCount, Oop *roots);

/**
 * I am an oop that is present in a member function.
 */
struct NYAST_CORE_EXPORT MemberOop : Oop
{
    MemberOop() {}
    MemberOop(const Oop &other)
        : Oop(other)
    {
    }

    MemberOop(const MemberOop &other)
        : Oop(other)
    {
    }

    MemberOop(const MemberOop &&other)
        : Oop(other)
    {
    }

    const MemberOop &operator=(const Oop &o)
    {
        value = o.value;
        return *this;
    }

    const MemberOop &operator=(const MemberOop &o)
    {
        value = o.value;
        return *this;
    }

    const MemberOop &operator=(const MemberOop &&o)
    {
        value = o.value;
        return *this;
    }
};

struct NYAST_CORE_EXPORT RootOop : Oop
{
    RootOop()
        : Oop(0)
    {
        registerGlobalOopRoots(1, this);
    }

    ~RootOop()
    {
        unregisterGlobalOopRoots(1, this);
    }

    const RootOop &operator=(const Oop &o)
    {
        value = o.value;
        return *this;
    }

    const RootOop &operator=(const RootOop &o)
    {
        value = o.value;
        return *this;
    }

    const RootOop &operator=(const RootOop &&o)
    {
        value = o.value;
        return *this;
    }
};

template<typename T>
struct GCReferenceTypeForFieldType
{
    static constexpr GCReferenceType value = GCReferenceType::Value;
};

template<>
struct GCReferenceTypeForFieldType<Oop>
{
    static constexpr GCReferenceType value = GCReferenceType::Strong;
};

template<>
struct GCReferenceTypeForFieldType<MemberOop> : GCReferenceTypeForFieldType<Oop> {};


/**
 * For conservative garbage collection, we need to ensure that registers are at
 * least spilled once onto the stack.
 */
struct StackSpilledRegisters
{
    uintptr_t pointerRegisters[16];
    uintptr_t endOfStruct;
};

struct StackRangeRecord
{
    StackSpilledRegisters spilledRegisters;
    StackRangeRecord *previous;
    bool isActive;
    const void *startAddress;
    const void *endAddress;
};

/**
 * This function cannot be inlined, and must be defined in a separate compilation unit.
 */
NYAST_CORE_EXPORT StackSpilledRegisters spillRegistersOntoStack(
    uintptr_t arg0=0, uintptr_t arg1=0, uintptr_t arg2=0, uintptr_t arg3=0,
    uintptr_t arg4=0, uintptr_t arg5=0, uintptr_t arg6=0, uintptr_t arg7=0,
    uintptr_t arg8=0, uintptr_t arg9=0, uintptr_t arg10=0, uintptr_t arg11=0,
    uintptr_t arg12=0, uintptr_t arg13=0, uintptr_t arg14=0, uintptr_t arg15=0
);

NYAST_CORE_EXPORT StackRangeRecord *getCurrentStackRangeRecord();
NYAST_CORE_EXPORT void activateStackRangeRecord(StackRangeRecord *record);
NYAST_CORE_EXPORT void deactivateStackRangeRecord(StackRangeRecord *record);
NYAST_CORE_EXPORT void deactivateCurrentStackRangeRecordAtAddress(const void *address);
NYAST_CORE_EXPORT void reactivateCurrentStackRangeRecordAtAddress(const void *address);
NYAST_CORE_EXPORT uint8_t *allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void (uint8_t*)> &memoryInitializationFunction);
NYAST_CORE_EXPORT void gcSafePoint();

struct NYAST_CORE_EXPORT ActivateStackRangeRecord
{
    ActivateStackRangeRecord(StackRangeRecord *crecord)
        : record(crecord)
    {
        activateStackRangeRecord(record);
    }

    ~ActivateStackRangeRecord()
    {
        deactivateStackRangeRecord(record);
    }

    StackRangeRecord *record;
};

struct NYAST_CORE_EXPORT DeactiveCurrentStackRangeRecord
{
    DeactiveCurrentStackRangeRecord(const void *cendAddress)
        : endAddress(cendAddress)
    {
        deactivateCurrentStackRangeRecordAtAddress(endAddress);
    }

    ~DeactiveCurrentStackRangeRecord()
    {
        reactivateCurrentStackRangeRecordAtAddress(endAddress);
    }

    const void *endAddress;
};

template<typename FT>
inline auto withGarbageCollectedStack(const FT &f)
{
    auto rangeRecord = StackRangeRecord{};
    rangeRecord.spilledRegisters = spillRegistersOntoStack();
    rangeRecord.startAddress = &rangeRecord.spilledRegisters.endOfStruct;

    ActivateStackRangeRecord barrier(&rangeRecord);
    return f();
}

template<typename FT>
inline auto withoutGarbageCollectedStack(const FT &f)
{
    auto spilledRegisters = spillRegistersOntoStack();
    DeactiveCurrentStackRangeRecord barrier(&spilledRegisters.endOfStruct);
    return f();
}

template<>
struct CppToOopAbi<Oop>
{
    typedef AbiOop type;

    AbiOop operator()(const Oop &oop)
    {
        return oop.value;
    }
};

template<>
struct CppToOopAbi<MemberOop> : CppToOopAbi<Oop> {};

template<>
struct OopAbiToCpp<Oop>
{
    Oop operator()(AbiOop value)
    {
        return Oop(value);
    }
};

template<>
struct OopAbiToCpp<MemberOop> : OopAbiToCpp<Oop> {};

/**
 * I am a definition of the nyast object vtable.
 */
struct NyastObjectVTable
{
    // Construction / finalization
    void (*basicInitialize) (AbiOop self);
    Oop (*initialize) (AbiOop self);
    void (*finalize) (AbiOop self);

    // Reflection core.
    Oop (*getClass) (AbiOop self);
    Oop (*lookupSelector)(AbiOop self, Oop selector);
    Oop (*runWithIn) (AbiOop self, Oop selector, const OopList &marshalledArguments, Oop receiver);
    MethodLookupResult (*asMethodLookupResult)(AbiOop self, MessageDispatchTrampolineSet trampolineSet);

    void (*addSubclass) (AbiOop self, Oop subclass);

    Oop (*basicNewInstance) (AbiOop self);
    Oop (*basicNewInstanceWithVariableSize) (AbiOop self, size_t variableSize);
    Oop (*newInstance) (AbiOop self);
    Oop (*newInstanceWithVariableSize) (AbiOop self, size_t variableSize);

    Oop (*getClassLayout) (AbiOop self);
    Oop (*getSlotScope) (AbiOop self);
    Oop (*getGCLayout) (AbiOop self);

    // GC Layout.
    void (*setGCReferenceTypeAtOffsetSize)(AbiOop self, size_t offset, size_t valueSize, GCReferenceType value);

    // Slots.
    Oop (*getName) (AbiOop self);
    Oop (*read) (AbiOop self, Oop receiver);
    Oop (*writeTo) (AbiOop self, Oop value, Oop receiver);
    void (*storeReferenceTypesInGCLayout) (AbiOop self, Oop gcLayout);

    // Basic operations
    OopHash (*identityHash)(AbiOop self);
    bool (*identityEquals)(AbiOop self, Oop other);
    OopHash (*hash)(AbiOop self);
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
    Oop (*evaluateValueWithArg)(AbiOop self, Oop value);

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

    Oop initialize()
    {
        return __vtable()->initialize(abiSelf());
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

    void addSubclass(Oop subclass)
    {
        return __vtable()->addSubclass(abiSelf(), subclass);
    }

    Oop basicNewInstance()
    {
        return __vtable()->basicNewInstance(abiSelf());
    }

    Oop basicNewInstance(size_t variableDataSize)
    {
        return __vtable()->basicNewInstanceWithVariableSize(abiSelf(), variableDataSize);
    }

    Oop newInstance()
    {
        return __vtable()->newInstance(abiSelf());
    }

    Oop newInstance(size_t variableDataSize)
    {
        return __vtable()->newInstanceWithVariableSize(abiSelf(), variableDataSize);
    }

    Oop getClassLayout()
    {
        return __vtable()->getClassLayout(abiSelf());
    }

    Oop getSlotScope()
    {
        return __vtable()->getSlotScope(abiSelf());
    }

    Oop getGCLayout()
    {
        return __vtable()->getGCLayout(abiSelf());
    }

    void setGCReferenceTypeAtOffsetSize(size_t offset, size_t valueSize, GCReferenceType value)
    {
        return __vtable()->setGCReferenceTypeAtOffsetSize(abiSelf(), offset, valueSize, value);
    }

    Oop getName()
    {
        return __vtable()->getName(abiSelf());
    }

    Oop read(Oop receiver)
    {
        return __vtable()->read(abiSelf(), receiver);
    }

    Oop writeTo(Oop value, Oop receiver)
    {
        return __vtable()->writeTo(abiSelf(), value, receiver);
    }

    void storeReferenceTypesInGCLayout(Oop gcLayout)
    {
        __vtable()->storeReferenceTypesInGCLayout(abiSelf(), gcLayout);
    }

    // Basic operations.
    OopHash identityHash() const
    {
        return __vtable()->identityHash(abiSelf());
    }

    bool identityEquals(Oop other) const
    {
        return __vtable()->identityEquals(abiSelf(), other);
    }

    OopHash hash() const
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

    Oop getValue() const
    {
        return evaluateValue();
    }

    void setValue(Oop value)
    {
        evaluateValueWithArg(value);
    }

    Oop evaluateValue() const
    {
        return __vtable()->evaluateValue(abiSelf());
    }

    Oop evaluateValueWithArg(Oop arg)
    {
        return __vtable()->evaluateValueWithArg(abiSelf(), arg);
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

template<typename T>
struct CppToOop<T&> : CppToOop<T> {};

template<typename T>
struct CppToOop<T&&> : CppToOop<T> {};

template<>
struct CppToOop<Oop>
{
    Oop operator()(Oop v)
    {
        return v;
    }
};

template<>
struct CppToOop<MemberOop>
{
    Oop operator()(MemberOop v)
    {
        return v.asOop();
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

template<typename T>
struct OopToCpp<T, typename std::enable_if<std::is_base_of<NyastObject, T>::value>::type> : OopToCpp<T*> {};

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
struct CppToOop<GCReferenceType>
{
    Oop operator()(GCReferenceType v)
    {
        return Oop::fromInt32(int32_t(v));
    }
};

template<>
struct OopToCpp<MemberOop> : OopToCpp<Oop> {};

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

template<typename T>
struct OopToCpp<const T &> : OopToCpp<T> {};

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

template<>
struct OopToCpp<GCReferenceType>
{
    GCReferenceType operator()(Oop v)
    {
        return GCReferenceType(v.decodeSmallInteger());
    }
};

template<typename T>
struct CppMemberToAccessorArgumentType
{
    typedef T type;
};

template<>
struct CppMemberToAccessorArgumentType<MemberOop>
{
    typedef Oop type;
};

template<typename RT, typename...Args>
struct NativeMethodDispatchTrampoline
{
    static RT value(void *methodFunctionPointer, Oop selector, Oop self, typename CppToOopAbi<Args>::type... args)
    {
        (void)selector;
        return reinterpret_cast<RT (*) (Oop, typename CppToOopAbi<Args>::type...)> (methodFunctionPointer) (self, args...);
    }
};

template<typename ResultType, typename...Args>
struct ObjectMethodDispatchTrampoline
{
    static ResultType value(void *methodObject, Oop selector, Oop self, typename CppToOopAbi<Args>::type... args)
    {
        auto methodOop = Oop::fromObjectPtr(reinterpret_cast<NyastObject*> (methodObject));
        OopList marshalledArguments{
            CppToOop<Args> ()(OopAbiToCpp<Args>()(args))...
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
ResultType Oop::typedPerformInSuperclass(InlineCache *inlineCache, Oop clazz, Oop typedSelector, Args&&... args)
{
    (void)inlineCache;
    gcSafePoint();
    auto foundMethod = clazz->lookupSelector(typedSelector);
    if(foundMethod.isNil())
        foundMethod = clazz;
    auto lookupResult = foundMethod->asMethodLookupResult(MessageDispatchTrampolineSet::forSignature<ResultType, Args...> ());
    return lookupResult.template apply<ResultType> (typedSelector, *this, std::forward<Args> (args)...);
}

template<typename ResultType, typename... Args>
ResultType Oop::performInSuperclass(InlineCache *inlineCache, Oop clazz, Oop selector, Args&&... args)
{
    return OopToCpp<ResultType> ()(
        typedPerformInSuperclass<Oop> (inlineCache, clazz, selector,
            CppToOop<Args> ()(std::forward<Args> (args))...
        )
    );
}

} // End of namespace nyast
#endif //NYAST_OBJECT_HPP
