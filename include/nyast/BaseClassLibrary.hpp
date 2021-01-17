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

template<typename T>
T *makeInstance(size_t variableDataSize = 0)
{
    size_t allocationSize = sizeof(T) + T::variableDataElementSize * variableDataSize;
    char *allocation = new char[allocationSize] ();
    memset(allocation, 0, allocationSize);
    auto result = new (allocation) T;
    result->__variableDataSize = variableDataSize;
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

template<typename BT, typename ST>
struct Subclass : BT
{
    typedef BT Super;
    typedef ST SelfType;

    virtual NyastObject *getClass() const override
    {
        return StaticClassObjectFor<SelfType>::value().asObjectPtr();
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
};

struct ProtoObject : NyastObject
{
    typedef void Super;
    typedef ProtoObject SelfType;
    typedef void value_type;

    static constexpr size_t variableDataElementSize = 0;
    static constexpr char const __className__[] = "ProtoObject";

    virtual NyastObject *getClass() const override
    {
        return StaticClassObjectFor<SelfType>::value().asObjectPtr();
    }

    virtual Oop lookupSelector(Oop selector) const override;
    virtual Oop atOrNil(Oop key) const override;
    virtual Oop runWithIn(Oop selector, const OopList &marshalledArguments, Oop self) override;
    virtual MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const override;

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
    virtual std::vector<Oop> asStdOopList() const override;

    uint32_t __variableDataSize;
};

struct Object : Subclass<ProtoObject, Object>
{
    static constexpr char const __className__[] = "Object";
};

struct Behavior : Subclass<Object, Behavior>
{
    static constexpr char const __className__[] = "Behavior";

    virtual Oop lookupSelector(Oop selector) const override;

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

    Oop name;
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
};

struct False : Subclass<Boolean, False>
{
    static constexpr char const __className__[] = "False";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<False>::value();
    }
};

struct UndefinedObject : Subclass<Object, UndefinedObject>
{
    static constexpr char const __className__[] = "UndefinedObject";

    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<UndefinedObject>::value();
    }
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

struct HashedCollection : Subclass<Collection, HashedCollection>
{
    static constexpr char const __className__[] = "HashedCollection";
};


struct AbstractDictionary : Subclass<HashedCollection, AbstractDictionary>
{
    static constexpr char const __className__[] = "AbstractDictionary";

};

struct Dictionary : Subclass<AbstractDictionary, Dictionary>
{
    static constexpr char const __className__[] = "Dictionary";
};

struct MethodDictionary : Subclass<AbstractDictionary, MethodDictionary>
{
    static constexpr char const __className__[] = "MethodDictionary";

    virtual Oop atOrNil(Oop key) const override;

    std::unordered_map<Oop, Oop> elements;
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

template <typename T>
Oop StaticClassObjectFor<T>::value()
{
    if(oop.isNotNilOrNull())
        return oop;

    Oop superClass = staticClassObjectFor<typename T::Super> ();
    auto metaClass = makeInstance<Metaclass> ();
    auto clazz = makeInstance<Class> ();
    clazz->superclass = superClass;
    clazz->name = Oop::internSymbol(T::__className__);
    oop = Oop::fromObjectPtr(clazz);

    if(superClass.isNotNil())
        metaClass->superclass = Oop::fromObjectPtr(superClass.getClass());
    else
        metaClass->superclass = staticClassObjectFor<Class> (); // Short circuit

    return oop;
}

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_HPP
