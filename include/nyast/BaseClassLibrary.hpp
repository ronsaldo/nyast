#ifndef NYAST_BASE_CLASS_LIBRARY_HPP
#define NYAST_BASE_CLASS_LIBRARY_HPP

#include "Object.hpp"
#include <unordered_map>

namespace nyast
{

struct ProtoObject;
struct Behavior;
struct ClassDescription;
struct Class;
struct Metaclass;
struct String;

typedef std::pair<Oop, Oop> MethodBinding;
typedef std::vector<MethodBinding> MethodBindings;

template <typename T>
struct StaticClassVTableFor
{
    static const NyastObjectVTable value;
};

template<typename T>
const NyastObjectVTable StaticClassVTableFor<T>::value = {

    // Construction / finalization
    // basicInitialize.
    +[](AbiOop) {
        //new (reinterpret_cast<T*> (self)) T;
    },

    // initialize
    +[](AbiOop self) {
        reinterpret_cast<T*> (self)->initialize();
    },

    // finalize
    +[](AbiOop self) {
        reinterpret_cast<T*> (self)->~T();
    },

    // Reflection core.
    // class
    +[](AbiOop self) -> Oop {
        return reinterpret_cast<T*> (self)->getClass();
    },

    // lookupSelector:
    +[](AbiOop self, Oop selector)-> Oop {
        return reinterpret_cast<T*> (self)->lookupSelector(selector);
    },

    // run:with:in:
    +[](AbiOop self, Oop selector, const OopList &marshalledArguments, Oop receiver) -> Oop {
        return reinterpret_cast<T*> (self)->runWithIn(selector, marshalledArguments, receiver);
    },

    // asMethodLookupResultFor:
    +[](AbiOop self, MessageDispatchTrampolineSet trampolineSet) -> MethodLookupResult {
        return reinterpret_cast<T*> (self)->asMethodLookupResult(trampolineSet);
    },

    // Basic operations
    // identityHash
    +[](AbiOop self) -> size_t {
        return reinterpret_cast<T*> (self)->identityHash();
    },

    // ==
    +[](AbiOop self, Oop other) -> bool {
        return reinterpret_cast<T*> (self)->identityEquals(other);
    },

    // hash
    +[](AbiOop self) -> size_t {
        return reinterpret_cast<T*> (self)->hash();
    },

    // =
    +[](AbiOop self, Oop other) -> bool {
        return reinterpret_cast<T*> (self)->equals(other);
    },

    // Common collection methods.
    // basicSize
    +[](AbiOop self) -> size_t {
        return reinterpret_cast<T*> (self)->getBasicSize();
    },
    // basicAt:
    +[](AbiOop self, size_t key) -> Oop {
        return reinterpret_cast<T*> (self)->basicAt(key);
    },
    // basicAt:put:
    +[](AbiOop self, size_t key, Oop value) -> Oop {
        return reinterpret_cast<T*> (self)->basicAtPut(key, value);
    },

    // size
    +[](AbiOop self) -> size_t {
        return reinterpret_cast<T*> (self)->getSize();
    },
    // at:
    +[](AbiOop self, Oop key) -> Oop {
        return reinterpret_cast<T*> (self)->at(key);
    },
    // at:put:
    +[](AbiOop self, Oop key, Oop value) -> Oop {
        return reinterpret_cast<T*> (self)->atPut(key, value);
    },
    // atOrNil:
    +[](AbiOop self, Oop key) -> Oop {
        return reinterpret_cast<T*> (self)->atOrNil(key);
    },

    // scanFor:
    +[](AbiOop self, Oop key) -> Oop {
        return reinterpret_cast<T*> (self)->scanFor(key);
    },

    // grow
    +[](AbiOop self) -> void {
        return reinterpret_cast<T*> (self)->grow();
    },

    // add:
    +[](AbiOop self, Oop value) -> Oop {
        return reinterpret_cast<T*> (self)->add(value);
    },

    // Association
    // key
    +[](AbiOop self) -> Oop {
        return reinterpret_cast<T*> (self)->getKey();
    },

    // Blocks
    // value
    +[](AbiOop self) -> Oop {
        return reinterpret_cast<T*> (self)->evaluateValue();
    },

    // Testing methods.
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isArray();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isAssociation();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isBehavior();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isBlock();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isCharacter();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isDictionary();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isFloat();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isFraction();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isInteger();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isInterval();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isNumber();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isString();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->isSymbol();
    },

    // Basic conversions
    +[](AbiOop self) -> std::string {
        return reinterpret_cast<T*> (self)->asString();
    },
    +[](AbiOop self) -> std::string {
        return reinterpret_cast<T*> (self)->printString();
    },
    +[](AbiOop self) -> bool {
        return reinterpret_cast<T*> (self)->asBoolean8();
    },
    +[](AbiOop self) -> uint32_t {
        return reinterpret_cast<T*> (self)->asUInt32();
    },
    +[](AbiOop self) -> int32_t {
        return reinterpret_cast<T*> (self)->asInt32();
    },
    +[](AbiOop self) -> uint64_t {
        return reinterpret_cast<T*> (self)->asUInt64();
    },
    +[](AbiOop self) -> int64_t {
        return reinterpret_cast<T*> (self)->asInt64();
    },
    +[](AbiOop self) -> char32_t {
        return reinterpret_cast<T*> (self)->asChar32();
    },
    +[](AbiOop self) -> double {
        return reinterpret_cast<T*> (self)->asFloat64();
    },
    +[](AbiOop self) -> std::string {
        return reinterpret_cast<T*> (self)->asStdString();
    },
    +[](AbiOop self) -> OopList {
        return reinterpret_cast<T*> (self)->asOopList();
    },
    +[](AbiOop self) -> ByteArrayData {
        return reinterpret_cast<T*> (self)->asByteArrayData();
    },
};

template<typename T>
typename T::value_type *variableDataOf(T *self)
{
    return reinterpret_cast<typename T::value_type*> (&self[1]);
}

template<typename T>
const typename T::value_type *variableDataOf(const T *self)
{
    return reinterpret_cast<const typename T::value_type*> (&self[1]);
}

template<typename T, typename... Args>
T *basicNewInstance(size_t variableDataSize = 0, Args&&... args)
{
    size_t allocationSize = sizeof(T) + T::variableDataElementSize * variableDataSize;
    char *allocation = new char[allocationSize] ();
    memset(allocation, 0, allocationSize);

    auto result = reinterpret_cast<T*> (allocation);

    // Basic initialize the object.
    new (result) T(args...);

    // Set the object vtable.
    reinterpret_cast<NyastObject*> (allocation)->__vtable = &StaticClassVTableFor<T>::value;

    // Initialize the variable data.
    result->__variableDataSize = uint32_t(variableDataSize);
    if constexpr(T::variableDataElementSize > 0)
        new (variableDataOf(result)) typename T::value_type[variableDataSize];
    return result;
}

template<typename T, typename... Args>
T *newInstance(size_t variableDataSize = 0, Args&&... args)
{
    auto result = basicNewInstance<T> (variableDataSize, args...);
    result->initialize();
    return result;
}

template <typename T>
struct ObjectSingletonInstanceOf
{
    static T instance;

    static Oop value()
    {
        return Oop::fromObjectPtr(&instance);
    }

    static T makeSingleton()
    {
        T result;
        result.__vtable = &StaticClassVTableFor<T>::value;
        return result;
    }
};

template<typename T>
T ObjectSingletonInstanceOf<T>::instance = makeSingleton();

template <typename T>
struct StaticClassObjectFor
{
    static Oop oop;

    static Oop value();
};

template <>
struct StaticClassObjectFor<void>
{
    static Oop value()
    {
        return Oop::nil();
    }
};

template<typename T>
Oop StaticClassObjectFor<T>::oop = Oop();

template<typename T>
Oop staticClassObjectFor ()
{
    return StaticClassObjectFor<T>::value();
}

template<typename BT, typename ST>
struct Subclass : BT
{
    typedef BT Super;
    typedef ST SelfType;

    Oop getClass() const
    {
        return StaticClassObjectFor<SelfType>::value();
    }

    static MethodBindings __instanceMethods__()
    {
        return MethodBindings{};
    }

    static MethodBindings __classMethods__()
    {
        return MethodBindings{};
    }
};

template<typename ST, typename SelfType, typename ET>
struct SubclassWithVariableDataOfType : Subclass<ST, SelfType>
{
    typedef ET value_type;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;

    static constexpr size_t variableDataElementSize = sizeof(value_type);

    value_type *variableData()
    {
        return variableDataOf(static_cast<SelfType*> (this));
    }

    const value_type *variableData() const
    {
        return variableDataOf(static_cast<const SelfType*> (this));
    }

    iterator begin()
    {
        return variableDataOf(static_cast<SelfType*> (this));
    }

    const_iterator begin() const
    {
        return variableDataOf(static_cast<const SelfType*> (this));
    }

    iterator end()
    {
        return variableDataOf(static_cast<SelfType*> (this)) + this->__variableDataSize;
    }

    const_iterator end() const
    {
        return variableDataOf(static_cast<const SelfType*> (this)) + this->__variableDataSize;
    }

    size_t size() const
    {
        return this->__variableDataSize;
    }

    bool empty() const
    {
        return size() == 0;
    }

    size_t getBasicSize() const
    {
        return size();
    }

    Oop basicAt(size_t index) const
    {
        if(index < 1 || index > size())
            throw std::runtime_error("Out of bounds.");

        return CppToOop<value_type>() (variableData()[index - 1]);
    }

    Oop basicAtPut(size_t index, Oop value)
    {
        if(index < 1 || index > size())
            throw std::runtime_error("Out of bounds.");

        variableData()[index - 1] = OopToCpp<value_type> ()(value);
        return value;
    }
};

template<typename ST, typename SelfType>
struct SubclassWithImmediateRepresentation : Subclass<ST, SelfType>
{

};

struct ProtoObject : Subclass<NyastObject, ProtoObject>
{
    typedef void Super;
    typedef void value_type;

    static constexpr size_t variableDataElementSize = 0;
    static constexpr char const __className__[] = "ProtoObject";

    static MethodBindings __instanceMethods__();
    static MethodBindings __classMethods__();

    ~ProtoObject();
    void initialize();

    Oop lookupSelector(Oop selector) const;
    Oop runWithIn(Oop selector, const OopList &marshalledArguments, Oop self);
    MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const;

    size_t identityHash() const;
    bool identityEquals(Oop other) const;
    size_t hash() const;
    bool equals(Oop other) const;

    size_t getBasicSize() const;
    Oop basicAt(size_t index) const;
    Oop basicAtPut(size_t index, Oop value);
    size_t getSize() const;
    Oop at(Oop key) const;
    Oop atPut(Oop key, Oop value);

    Oop atOrNil(Oop key) const;
    Oop scanFor(Oop key) const;
    void grow();
    Oop add(Oop value);

    // Association
    Oop getKey() const;

    // Blocks
    Oop evaluateValue() const;

    // Testing methods
    bool isArray() const;
    bool isAssociation() const;
    bool isBehavior() const;
    bool isBlock() const;
    bool isCharacter() const;
    bool isDictionary() const;
    bool isFloat() const;
    bool isFraction() const;
    bool isInteger() const;
    bool isInterval() const;
    bool isNumber() const;
    bool isString() const;
    bool isSymbol() const;

    std::string asString() const;
    std::string printString() const;

    // Basic conversions
    bool asBoolean8() const;
    uint32_t asUInt32() const;
    int32_t asInt32() const;
    uint64_t asUInt64() const;
    int64_t asInt64() const;
    char32_t asChar32() const;
    double asFloat64() const;
    std::string asStdString() const;
    std::vector<Oop> asOopList() const;
    ByteArrayData asByteArrayData() const;

    uint32_t __variableDataSize;

private:
    Oop yourself();
    Oop doesNotUnderstand(Oop message);
};

struct Object : Subclass<ProtoObject, Object>
{
    static constexpr char const __className__[] = "Object";

    static MethodBindings __instanceMethods__();

    // Testing methods
    bool isArray() const;
    bool isAssociation() const;
    bool isBehavior() const;
    bool isBlock() const;
    bool isCharacter() const;
    bool isDictionary() const;
    bool isFloat() const;
    bool isFraction() const;
    bool isInteger() const;
    bool isInterval() const;
    bool isNumber() const;
    bool isString() const;
    bool isSymbol() const;

    // Errors.
    Oop error();
    Oop error(const std::string &errorMessage);
    Oop explicitRequirement();
    Oop subclassResponsibility();
    Oop shouldBeImplemented();
    Oop shouldNotImplement();
};

struct Behavior : Subclass<Object, Behavior>
{
    static constexpr char const __className__[] = "Behavior";
    static MethodBindings __instanceMethods__();

    void initialize();
    Oop lookupSelector(Oop selector) const;
    Oop runWithIn(Oop selector, const OopList &marshalledArguments, Oop self);

    void addMethodBindings(const MethodBindings &methods);

    Oop superclass;
    Oop methodDict;
};

struct ClassDescription : Subclass<Behavior, ClassDescription>
{
    static constexpr char const __className__[] = "ClassDescription";
};

struct Class : Subclass<ClassDescription, Class>
{
    static constexpr char const __className__[] = "Class";

    std::string asString() const;
    Oop getClass() const;

    Oop name;
    Oop metaClass;
};

struct Metaclass : Subclass<ClassDescription, Metaclass>
{
    static constexpr char const __className__[] = "Metaclass";

    std::string asString() const;

    Oop thisClass;
};

struct Boolean : Subclass<Object, Boolean>
{
    static constexpr char const __className__[] = "Boolean";
};

struct True : Subclass<Boolean, True>
{
    static constexpr char const __className__[] = "True";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<True>::value();
    }

    std::string asString() const;
};

struct False : Subclass<Boolean, False>
{
    static constexpr char const __className__[] = "False";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<False>::value();
    }

    std::string asString() const;
};

struct UndefinedObject : Subclass<Object, UndefinedObject>
{
    static constexpr char const __className__[] = "UndefinedObject";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<UndefinedObject>::value();
    }

    std::string asString() const;
};

struct Collection : Subclass<Object, Collection>
{
    static constexpr char const __className__[] = "Collection";
};

struct SequenceableCollection : Subclass<Collection, SequenceableCollection>
{
    static constexpr char const __className__[] = "SequenceableCollection";
};

struct ArrayedCollection : Subclass<SequenceableCollection, ArrayedCollection>
{
    static constexpr char const __className__[] = "ArrayedCollection";
};

struct Array : SubclassWithVariableDataOfType<ArrayedCollection, Array, Oop>
{
    static constexpr char const __className__[] = "Array";

    std::vector<Oop> asOopList() const;
};

struct ByteArray : SubclassWithVariableDataOfType<ArrayedCollection, ByteArray, uint8_t>
{
    static constexpr char const __className__[] = "ByteArray";

    ByteArrayData asByteArrayData() const;
    std::string asString() const;
    std::string printString() const;
};

struct String : SubclassWithVariableDataOfType<ArrayedCollection, String, char>
{
    static constexpr char const __className__[] = "String";

    std::string asStdString() const;
    std::string asString() const;
    std::string printString() const;
};

struct Symbol : Subclass<String, Symbol>
{
    static constexpr char const __className__[] = "Symbol";

    std::string printString() const;
};

struct Association : Subclass<Object, Association>
{
    static constexpr char const __className__[] = "Association";

    Oop getKey() const;
    Oop evaluateValue() const;

    Oop key;
    Oop value;
};

struct Message : Subclass<Object, Message>
{
    static constexpr char const __className__[] = "Message";

    std::string printString() const;

    Oop selector;
    Oop args;
    Oop lookupClass;
};

struct HashedCollection : Subclass<Collection, HashedCollection>
{
    static constexpr char const __className__[] = "HashedCollection";

    void initialize();
    void fullCheck();

    size_t tally = 0;
    Oop array;
};

struct Dictionary : Subclass<HashedCollection, Dictionary>
{
    static constexpr char const __className__[] = "Dictionary";

    Oop atOrNil(Oop key) const;
};

struct MethodDictionary : Subclass<Dictionary, MethodDictionary>
{
    static constexpr char const __className__[] = "MethodDictionary";

    void initialize();

    Oop scanFor(Oop key) const;
    Oop atOrNil(Oop key) const;

    Oop atPut(Oop key, Oop value);
    void grow();

    Oop methods;
};

struct Magnitude : Subclass<Object, Magnitude>
{
    static constexpr char const __className__[] = "Magnitude";
};

struct Character : SubclassWithImmediateRepresentation<Magnitude, Character>
{
    static constexpr char const __className__[] = "Character";

    std::string asString() const;
    char32_t asChar32() const;
};

struct Number : Subclass<Magnitude, Number>
{
    static constexpr char const __className__[] = "Number";

    static MethodBindings __instanceMethods__();

    bool isNumber() const;

    Oop additionWith(Oop other);
    Oop subtractionWith(Oop other);
    Oop multiplicationWith(Oop other);
    Oop divisionWith(Oop other);
};

struct Fraction : Subclass<Number, Fraction>
{
    static constexpr char const __className__[] = "Fraction";

    static Oop constructWithNumeratorDenominator(Oop numerator, Oop denominator);

    double asFloat64();

    Oop numerator;
    Oop denominator;
};

struct Integer : Subclass<Number, Integer>
{
    static constexpr char const __className__[] = "Integer";
};

struct LargeInteger : SubclassWithVariableDataOfType<Integer, LargeInteger, uint8_t>
{
    static constexpr char const __className__[] = "LargeInteger";

    static Oop createWithSignAndUnormalizedData(bool isNegative, size_t dataSize, const uint8_t *data);

};

struct LargePositiveInteger : Subclass<LargeInteger, LargePositiveInteger>
{
    static constexpr char const __className__[] = "LargePositiveInteger";

    static Oop createWithUnormalizedData(size_t dataSize, const uint8_t *data);

    uint64_t asUInt64() const;
    int64_t asInt64() const;
    double asFloat64() const;
};

struct LargeNegativeInteger : Subclass<LargeInteger, LargeNegativeInteger>
{
    static constexpr char const __className__[] = "LargeNegativeInteger";

    static Oop createWithUnormalizedData(size_t dataSize, const uint8_t *data);

    int64_t asInt64() const;
    double asFloat64() const;
};

struct SmallInteger : SubclassWithImmediateRepresentation<Integer, SmallInteger>
{
    static constexpr char const __className__[] = "SmallInteger";

    static MethodBindings __instanceMethods__();

    std::string asString() const;

    uint32_t asUInt32() const;
    int32_t asInt32() const;
    uint64_t asUInt64() const;
    int64_t asInt64() const;
    double asFloat64() const;

    Oop additionWith(Oop other);
    Oop subtractionWith(Oop other);
    Oop multiplicationWith(Oop other);
    Oop divisionWith(Oop other);
};

struct Float : Subclass<Number, Float>
{
    static constexpr char const __className__[] = "Float";
};

struct SmallFloat64 : SubclassWithImmediateRepresentation<Float, SmallFloat64>
{
    static constexpr char const __className__[] = "SmallFloat64";

    static MethodBindings __instanceMethods__();

    std::string asString() const;
    double asFloat64() const;

    Oop additionWith(Oop other);
    Oop subtractionWith(Oop other);
    Oop multiplicationWith(Oop other);
    Oop divisionWith(Oop other);
};

struct BoxedFloat64 : Subclass<Float, BoxedFloat64>
{
    static constexpr char const __className__[] = "BoxedFloat64";

    static MethodBindings __instanceMethods__();

    std::string asString() const;
    double asFloat64() const;

    Oop additionWith(Oop other);
    Oop subtractionWith(Oop other);
    Oop multiplicationWith(Oop other);
    Oop divisionWith(Oop other);

    double value;


};

template <typename T>
Oop StaticClassObjectFor<T>::value()
{
    if(oop.isNotNilOrNull())
        return oop;

    auto metaClass = newInstance<Metaclass> ();
    auto clazz = newInstance<Class> ();
    clazz->metaClass = Oop::fromObjectPtr(metaClass);
    metaClass->thisClass = Oop::fromObjectPtr(clazz);
    oop = Oop::fromObjectPtr(clazz);

    Oop superClass = staticClassObjectFor<typename T::Super> ();
    clazz->superclass = superClass;

    if(superClass.isNotNil())
        metaClass->superclass = superClass.getClass();
    else
        metaClass->superclass = staticClassObjectFor<Class> (); // Short circuit

    clazz->name = Oop::internSymbol(T::__className__);
    clazz->addMethodBindings(T::__instanceMethods__());
    metaClass->addMethodBindings(T::__classMethods__());

    return oop;
}

/**
 * I am a direct native method.
 */
struct NativeMethod : Subclass<Object, NativeMethod>
{
    static constexpr char const __className__[] = "NativeMethod";

    MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const;

    Oop selector;
    void *entryPoint;
};

/**
 * I am the base class for a cpp functional method.
 */
struct CppMethodBindingBase : Subclass<Object, CppMethodBindingBase>
{
    static constexpr char const __className__[] = "CppMethodBinding";

    CppMethodBindingBase(Oop cselector)
        : selector(cselector) {}

    Oop selector;
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

    static Oop trampoline(SelfType *methodBinding, Oop, CppTypeToOopType<Args>... args)
    {
        if constexpr(std::is_same<ResultType, void>::value)
        {
            methodBinding->functor(OopToCpp<Args> ()(args)...);
            return Oop::nil();
        }
        else
        {
            return CppToOop<ResultType>() (methodBinding->functor(OopToCpp<Args> ()(args)...));
        }
    }

    MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet) const
    {
        return MethodLookupResult{const_cast<SelfType*> (this), reinterpret_cast<void*> (&trampoline)};
    }

    FT functor;
};


template<typename FT>
MethodBinding makeRawNativeMethodBinding(const std::string &selector, const FT &function)
{
    auto selectorOop = Oop::internSymbol(selector);
    auto nativeMethod = basicNewInstance<NativeMethod> ();
    nativeMethod->selector = selectorOop;
    nativeMethod->entryPoint = reinterpret_cast<void*> (function);
    return MethodBinding{selectorOop, Oop::fromObjectPtr(nativeMethod)};
}

template<typename MethodSignature, typename FT>
MethodBinding makeMethodBinding(const std::string &selector, FT &&functor)
{
    auto selectorOop = Oop::internSymbol(selector);
    auto methodBinding = basicNewInstance<CppMethodBinding<MethodSignature, FT> > (0, selectorOop, functor);
    return MethodBinding{selectorOop, Oop::fromObjectPtr(methodBinding)};
}

template<typename SelfType, typename ResultType, typename... Args>
MethodBinding makeMethodBinding(const std::string &selector, ResultType (SelfType::*memberFunction)(Args...))
{
    return makeMethodBinding<ResultType (SelfType*, Args...)> (selector, [=](SelfType* self, Args&&... args){
        return (self->*memberFunction)(args...);
    });
}

template<typename SelfType, typename ResultType, typename... Args>
MethodBinding makeMethodBinding(const std::string &selector, ResultType (SelfType::*memberFunction)(Args...) const)
{
    return makeMethodBinding<ResultType (const SelfType*, Args...)> (selector, [=](const SelfType* self, Args&&... args){
        return (self->*memberFunction)(args...);
    });
}
} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_HPP
