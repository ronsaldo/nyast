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

template<typename T>
T *basicNewInstance(size_t variableDataSize = 0)
{
    size_t allocationSize = sizeof(T) + T::variableDataElementSize * variableDataSize;
    char *allocation = new char[allocationSize] ();
    memset(allocation, 0, allocationSize);
    auto result = new (allocation) T;
    result->__variableDataSize = uint32_t(variableDataSize);

    if constexpr(T::variableDataElementSize > 0)
        new (variableDataOf(result)) typename T::value_type[variableDataSize];
    return result;
}

template<typename T>
T *newInstance(size_t variableDataSize = 0)
{
    auto result = basicNewInstance<T> (variableDataSize);
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
};

template<typename T>
T ObjectSingletonInstanceOf<T>::instance;

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

    virtual NyastObject *getClass() const override
    {
        return StaticClassObjectFor<SelfType>::value().asObjectPtr();
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

    virtual size_t getBasicSize() const override
    {
        return size();
    }

    virtual Oop basicAt(size_t index) const
    {
        if(index < 1 || index > size())
            throw std::runtime_error("Out of bounds.");

        return CppToOop<value_type>() (variableData()[index - 1]);
    }

    virtual Oop basicAtPut(size_t index, Oop value)
    {
        if(index < 1 || index > size())
            throw std::runtime_error("Out of bounds.");

        variableData()[index - 1] = OopToCpp<value_type> ()(value);
        return value;
    }
};

struct ProtoObject : NyastObject
{
    typedef void Super;
    typedef ProtoObject SelfType;
    typedef void value_type;

    static constexpr size_t variableDataElementSize = 0;
    static constexpr char const __className__[] = "ProtoObject";

    static MethodBindings __instanceMethods__();
    static MethodBindings __classMethods__();

    virtual ~ProtoObject() override;
    virtual void initialize() override;

    virtual NyastObject *getClass() const override
    {
        return StaticClassObjectFor<SelfType>::value().asObjectPtr();
    }

    virtual Oop lookupSelector(Oop selector) const override;
    virtual Oop runWithIn(Oop selector, const OopList &marshalledArguments, Oop self) override;
    virtual MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const override;
    virtual Oop doesNotUnderstand(Oop message) override;

    virtual bool identityHash() const override;
    virtual bool identityEquals(Oop other) const override;
    virtual bool hash() const override;
    virtual bool equals(Oop other) const override;

    virtual size_t getBasicSize() const override;
    virtual Oop basicAt(size_t index) const override;
    virtual Oop basicAtPut(size_t index, Oop value) override;
    virtual size_t getSize() const override;
    virtual Oop at(Oop key) const override;
    virtual Oop atPut(Oop key, Oop value) override;

    virtual Oop atOrNil(Oop key) const override;
    virtual Oop scanFor(Oop key) const override;
    virtual void grow() override;
    virtual void add(Oop value) override;

    virtual Oop getKey() const override;
    virtual Oop getValue() const override;

    virtual std::string asString() const override;
    virtual std::string printString() const override;

    virtual bool asBoolean8() const override;
    virtual uint32_t asUInt32() const override;
    virtual int32_t asInt32() const override;
    virtual uint64_t asUInt64() const override;
    virtual int64_t asInt64() const override;
    virtual char32_t asChar32() const override;
    virtual double asFloat64() const override;
    virtual std::string asStdString() const override;
    virtual std::vector<Oop> asOopList() const override;
    virtual ByteArrayData asByteArrayData() const override;

    uint32_t __variableDataSize;
};

struct Object : Subclass<ProtoObject, Object>
{
    static constexpr char const __className__[] = "Object";
};

struct Behavior : Subclass<Object, Behavior>
{
    static constexpr char const __className__[] = "Behavior";
    static MethodBindings __instanceMethods__();

    virtual void initialize() override;
    virtual Oop lookupSelector(Oop selector) const override;
    virtual Oop runWithIn(Oop selector, const OopList &marshalledArguments, Oop self) override;

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

    virtual std::string asString() const override;
    virtual NyastObject *getClass() const override;

    Oop name;
    Oop metaClass;
};

struct Metaclass : Subclass<ClassDescription, Metaclass>
{
    static constexpr char const __className__[] = "Metaclass";

    virtual std::string asString() const override;

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

    virtual std::string asString() const override;
};

struct False : Subclass<Boolean, False>
{
    static constexpr char const __className__[] = "False";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<False>::value();
    }

    virtual std::string asString() const override;
};

struct UndefinedObject : Subclass<Object, UndefinedObject>
{
    static constexpr char const __className__[] = "UndefinedObject";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<UndefinedObject>::value();
    }

    virtual std::string asString() const override;
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

    virtual std::vector<Oop> asOopList() const override;
};

struct ByteArray : SubclassWithVariableDataOfType<ArrayedCollection, ByteArray, uint8_t>
{
    static constexpr char const __className__[] = "ByteArray";

    virtual ByteArrayData asByteArrayData() const override;
    virtual std::string asString() const override;
    virtual std::string printString() const override;
};

struct String : SubclassWithVariableDataOfType<ArrayedCollection, String, char>
{
    static constexpr char const __className__[] = "String";

    virtual std::string asStdString() const override;
    virtual std::string asString() const override;
    virtual std::string printString() const override;
};

struct Symbol : Subclass<String, Symbol>
{
    static constexpr char const __className__[] = "Symbol";

    virtual std::string printString() const override;
};

struct Association : Subclass<Object, Association>
{
    static constexpr char const __className__[] = "Association";

    virtual Oop getKey() const override;
    virtual Oop getValue() const override;

    Oop key;
    Oop value;
};

struct Message : Subclass<Object, Message>
{
    static constexpr char const __className__[] = "Message";

    virtual std::string printString() const override;

    Oop selector;
    Oop args;
    Oop lookupClass;
};

struct HashedCollection : Subclass<Collection, HashedCollection>
{
    static constexpr char const __className__[] = "HashedCollection";

    virtual void initialize() override;
    void fullCheck();

    size_t tally = 0;
    Oop array;
};

struct Dictionary : Subclass<HashedCollection, Dictionary>
{
    static constexpr char const __className__[] = "Dictionary";

    virtual Oop atOrNil(Oop key) const override;
};

struct MethodDictionary : Subclass<Dictionary, MethodDictionary>
{
    static constexpr char const __className__[] = "MethodDictionary";

    virtual void initialize() override;

    virtual Oop scanFor(Oop key) const override;
    virtual Oop atOrNil(Oop key) const override;

    virtual Oop atPut(Oop key, Oop value) override;
    virtual void grow() override;

    Oop methods;
};

struct Magnitude : Subclass<Object, Magnitude>
{
    static constexpr char const __className__[] = "Magnitude";
};

struct Character : Subclass<Magnitude, Character>
{
    static constexpr char const __className__[] = "Character";
};

struct Number : Subclass<Magnitude, Number>
{
    static constexpr char const __className__[] = "Number";
};

struct Integer : Subclass<Number, Integer>
{
    static constexpr char const __className__[] = "Integer";
};

struct LargeInteger : SubclassWithVariableDataOfType<Integer, LargeInteger, uint8_t>
{
    static constexpr char const __className__[] = "LargeInteger";
};

struct LargePositiveInteger : Subclass<LargeInteger, LargePositiveInteger>
{
    static constexpr char const __className__[] = "LargePositiveInteger";

    virtual uint64_t asUInt64() const override;
    virtual int64_t asInt64() const override;
    virtual double asFloat64() const override;
};

struct LargeNegativeInteger : Subclass<LargeInteger, LargeNegativeInteger>
{
    static constexpr char const __className__[] = "LargeNegativeInteger";

    virtual int64_t asInt64() const override;
    virtual double asFloat64() const override;
};

struct SmallInteger : Subclass<Integer, SmallInteger>
{
    static constexpr char const __className__[] = "SmallInteger";
};

struct Float : Subclass<Number, Float>
{
    static constexpr char const __className__[] = "Float";

};

struct SmallFloat64 : Subclass<Float, SmallFloat64>
{
    static constexpr char const __className__[] = "SmallFloat64";
};

struct BoxedFloat64 : Subclass<Float, BoxedFloat64>
{
    static constexpr char const __className__[] = "BoxedFloat64";

    virtual std::string asString() const override;
    virtual double asFloat64() const override;

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
        metaClass->superclass = Oop::fromObjectPtr(superClass.getClass());
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

    virtual MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const override;

    Oop selector;
    void *entryPoint;
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

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_HPP
