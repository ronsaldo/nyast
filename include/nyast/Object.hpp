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
struct Oop;
struct NyastObject;
struct NyastObjectVTable;

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
struct Oop : OopPointerSizeDependentImplementation<uintptr_t>
{
    static constexpr intptr_t SmallIntegerUsableBits = OopBits - SmallIntegerTagShift;
    static constexpr intptr_t SmallIntegerMinValue = -( ((intptr_t)1)<<(SmallIntegerUsableBits - 1) ) ;
    static constexpr intptr_t SmallIntegerMaxValue = +( ((intptr_t)1)<<(SmallIntegerUsableBits - 1) ) - 1;

    Oop();
    Oop(uintptr_t cvalue) : OopPointerSizeDependentImplementation{cvalue} {}

    static Oop nil();
    static Oop trueValue();
    static Oop falseValue();

    static Oop fromObjectPtr(NyastObject *ptr)
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

    static Oop fromSize(size_t value)
    {
        if constexpr(sizeof(uint32_t) == sizeof(size_t))
            return fromUInt32(uint32_t(value));
        else
            return fromUInt64(value);
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

    NyastObject *asObjectPtr() const
    {
        assert(isPointer());
        return reinterpret_cast<NyastObject*> (value);
    }

    NyastObject *operator->() const
    {
        return asObjectPtr();
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

    std::size_t hash() const;
    bool equals(Oop other) const;

    std::size_t identityHash() const;
    bool identityEquals(Oop other) const;

    bool asBoolean8() const;
    uint32_t asUInt32() const;
    int32_t asInt32() const;
    uint64_t asUInt64() const;
    int64_t asInt64() const;
    char32_t asChar32() const;
    double asFloat64() const;
    std::string asString() const;
    std::string printString() const;
    std::string asStdString() const;
    std::vector<Oop> asOopList() const;
    ByteArrayData asByteArrayData() const;

    NyastObject *getClass() const;
    std::string getClassName() const;

    template<typename ResultType, typename... Args>
    ResultType typedPerformInSuperclass(InlineCache *inlineCache, Oop clazz, Oop typedSelector, Args... args);

    template<typename ResultType, typename... Args>
    ResultType performInSuperclass(InlineCache *inlineCache, Oop clazz, Oop selector, Args... args);

    template<typename ResultType, typename... Args>
    ResultType typedPerform(InlineCache *inlineCache, Oop typedSelector, Args... args)
    {
        return typedPerformInSuperclass<ResultType> (inlineCache, Oop::fromObjectPtr(getClass()), typedSelector, args...);
    }

    template<typename ResultType, typename... Args>
    ResultType perform(InlineCache *inlineCache, Oop selector, Args... args)
    {
        return performInSuperclass<ResultType> (inlineCache, Oop::fromObjectPtr(getClass()), selector, args...);
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

    friend std::ostream &operator<<(std::ostream &out, const Oop &self)
    {
        out << self.asString();
        return out;
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

};

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
 * I am the mandatory interface that must be implemented by each managed non-immediate object.
 * I provide basic interface methods that are used for connecting the Smalltalk and C++ worlds.
 */
struct NyastObject
{
    virtual NyastObject *getClass() const = 0;
    virtual Oop lookupSelector(Oop selector) const = 0;
    virtual Oop runWithIn(Oop selector, const OopList &arguments, Oop self) = 0;
    virtual MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const = 0;

    // Basic operations
    virtual bool identityHash() const = 0;
    virtual bool identityEquals(Oop other) const = 0;
    virtual bool hash() const = 0;
    virtual bool equals(Oop other) const = 0;

    // Common collection methods.
    virtual Oop scanFor(Oop key) const = 0;
    virtual size_t getBasicSize() const = 0;
    virtual Oop basicAt(size_t index) const = 0;
    virtual Oop basicAtPut(size_t index, Oop value) = 0;
    virtual size_t getSize() const = 0;
    virtual Oop at(Oop key) const = 0;
    virtual Oop atPut(Oop key, Oop value) = 0;
    virtual Oop atOrNil(Oop key) const = 0;
    virtual Oop getKey() const = 0;
    virtual Oop getValue() const = 0;

    // Common conversion methods
    virtual std::string asString() const = 0;
    virtual std::string printString() const = 0;

    virtual bool asBoolean8() const = 0;
    virtual uint32_t asUInt32() const = 0;
    virtual int32_t asInt32() const = 0;
    virtual uint64_t asUInt64() const = 0;
    virtual int64_t asInt64() const = 0;
    virtual char32_t asChar32() const = 0;
    virtual double asFloat64() const = 0;
    virtual std::string asStdString() const = 0;
    virtual std::vector<Oop> asOopList() const = 0;
    virtual ByteArrayData asByteArrayData() const = 0;

    const NyastObjectVTable *getVTable() const
    {
        return reinterpret_cast<const NyastObjectVTable* const*> (this)[0];
    }

    void setVTable(const NyastObjectVTable *vtable)
    {
        reinterpret_cast<const NyastObjectVTable**> (this)[0] = vtable;
    }

    Oop asOop() const
    {
        return Oop::fromObjectPtr(const_cast<NyastObject*> (this));
    }
};

/**
 * I am an explicit declaration of a nyast object vtable.
 * I am used to allow creating new subclasses in the smalltalk world.
 */
struct NyastObjectVTable
{
    NyastObject *(getClass) (NyastObject *self);
    Oop (*lookupSelector)(NyastObject *self, Oop selector);
    Oop (*runWithIn) (Oop selector, const OopList &marshalledArguments, Oop self);
    MethodLookupResult (*asMethodLookupResult)(NyastObject *self, MessageDispatchTrampolineSet trampolineSet);

    // Basic operations
    bool (*identityHash)();
    bool (*identityEquals)(Oop other);
    bool (*hash)();
    bool (*equals)(Oop other);

    // Common collection methods.
    Oop (*scanFor)(Oop key);
    Oop (*getBasicSize)();
    Oop (*basicAt)(Oop key);
    Oop (*basicAtPut)(Oop key, Oop value);
    Oop (*getSize)();
    Oop (*at)(Oop key);
    Oop (*atPut)(Oop key, Oop value);
    Oop (*atOrNil)(Oop key);
    Oop (*getKey)();
    Oop (*getValue)();

    std::string (*asString)();
    std::string (*printString)();
    bool (*asBoolean8)();
    uint32_t (*asUInt32)();
    int32_t (*asInt32)();
    uint64_t (*asUInt64)();
    int64_t (*asInt64)();
    char32_t (*asChar32)();
    double (*asFloat64)();
    std::string (*asStdString)();
    std::vector<Oop> (*asOopList)();
    ByteArrayData (*asByteArrayData)();
};

inline bool Oop::identityEquals(Oop other) const
{
    if(isImmediate())
        return *this == other;
    return asObjectPtr()->identityEquals(other);
}

inline std::size_t Oop::identityHash() const
{
    if(isImmediate())
        return std::hash<uintptr_t> ()(value);

    return asObjectPtr()->identityHash();
}

inline bool Oop::equals(Oop other) const
{
    if(isPointer())
        return asObjectPtr()->equals(other);

    if(*this == other)
        return true;
    else if(other.isPointer())
        return other.asObjectPtr()->equals(*this);

    // SmallInteger = SmallFloat
    if(isSmallInteger())
    {
        if(other.isSmallFloat())
            return decodeSmallInteger() == other.decodeSmallFloat();
    }
    else if(isSmallFloat())
    {
        if(other.isSmallInteger())
            return decodeSmallFloat() == other.decodeSmallInteger();
    }

    return false;
}

inline std::size_t Oop::hash() const
{
    if(isImmediate())
        return std::hash<uintptr_t> ()(value);
    return asObjectPtr()->hash();
}

inline bool Oop::asBoolean8() const
{
    return asObjectPtr()->asBoolean8();
}

inline uint32_t Oop::asUInt32() const
{
    if(isSmallInteger())
        return static_cast<uint32_t> (decodeSmallInteger());
    return asObjectPtr()->asUInt32();
}

inline int32_t Oop::asInt32() const
{
    if(isSmallInteger())
        return static_cast<int32_t> (decodeSmallInteger());
    return asObjectPtr()->asInt32();
}

inline uint64_t Oop::asUInt64() const
{
    if(isSmallInteger())
        return static_cast<uint64_t> (decodeSmallInteger());
    return asObjectPtr()->asUInt64();
}

inline int64_t Oop::asInt64() const
{
    if(isSmallInteger())
        return static_cast<int64_t> (decodeSmallInteger());
    return asObjectPtr()->asInt64();
}

inline char32_t Oop::asChar32() const
{
    if(isCharacter())
        return decodeCharacter();

    return asObjectPtr()->asChar32();
}

inline double Oop::asFloat64() const
{
    if(isSmallFloat())
        return decodeSmallFloat();

    return asObjectPtr()->asFloat64();
}

inline std::string Oop::asStdString() const
{
    return asObjectPtr()->asStdString();
}

inline std::vector<Oop> Oop::asOopList() const
{
    return asObjectPtr()->asOopList();
}


inline ByteArrayData Oop::asByteArrayData() const
{
    return asObjectPtr()->asByteArrayData();
}

inline NyastObject *Oop::getClass() const
{
    if(isPointer())
        return reinterpret_cast<NyastObject*> (value)->getClass();

    return ImmediateClassTable[value & TagMask];
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
struct OopToCpp<T*, typename std::enable_if<std::is_base_of<NyastObject, T>::value>::type>
{
    Oop operator()(T* v)
    {
        return Oop::fromObjectPtr(v);
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
        return v.asBoolean8();
    }
};

template<>
struct OopToCpp<char32_t>
{
    uint32_t operator()(Oop v)
    {
        return v.asChar32();
    }
};

template<>
struct OopToCpp<uint32_t>
{
    uint32_t operator()(Oop v)
    {
        return v.asUInt32();
    }
};

template<>
struct OopToCpp<uint64_t>
{
    uint64_t operator()(Oop v)
    {
        return v.asUInt64();
    }
};

template<>
struct OopToCpp<int64_t>
{
    int64_t operator()(Oop v)
    {
        return v.asInt64();
    }
};

template<>
struct OopToCpp<double>
{
    double operator()(Oop v)
    {
        return v.asFloat64();
    }
};

template<>
struct OopToCpp<std::string>
{
    std::string operator()(Oop v)
    {
        return v.asStdString();
    }
};

template<>
struct OopToCpp<OopList>
{
    OopList operator()(Oop v)
    {
        return v.asOopList();
    }
};

template<typename T>
struct OopToCpp<T, typename std::enable_if<std::is_base_of<NyastObject, T>::value>::type>
{
    T *operator()(Oop v)
    {
        return static_cast<T*> (v.asObjectPtr());
    }
};

template<typename RT, typename...Args>
struct NativeMethodDispatchTrampoline
{
    static RT value(void *methodFunctionPointer, Oop selector, Oop self, Args... args)
    {
        (void)selector;
        return reinterpret_cast<RT (*) (void*, Args...)> (methodFunctionPointer) (reinterpret_cast<void*> (self.value), args...);
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

        if(methodOop.isPointer())
            return OopToCpp<ResultType>() (methodOop.asObjectPtr()->runWithIn(selector, marshalledArguments, self));
        return methodOop.perform<ResultType> ("run:with:in:", selector, marshalledArguments, self);
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
    auto foundMethod = clazz.asObjectPtr()->lookupSelector(typedSelector);
    if(foundMethod.isNil())
        foundMethod = clazz;
    auto lookupResult = foundMethod.asObjectPtr()->asMethodLookupResult(MessageDispatchTrampolineSet::forSignature<ResultType, Args...> ());
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
