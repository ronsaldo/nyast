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
T ObjectSingletonInstanceOf<T>::instance;

template<typename BT, typename ST>
struct Subclass : BT
{
    typedef BT SuperType;
    typedef ST SelfType;
};

template<typename BT, typename ST, typename ET>
struct SubclassWithVariableDataOfType : BT
{
    typedef BT SuperType;
    typedef ST SelfType;
    typedef ET value_type;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;

    static constexpr size_t variableDataElementSize = sizeof(value_type);

    value_type *variableData()
    {
        return variableDataOf(static_cast<ST*> (this));
    }

    iterator begin()
    {
        return variableDataOf(static_cast<ST*> (this));
    }

    const_iterator begin() const
    {
        return variableDataOf(static_cast<const ST*> (this));
    }

    iterator end()
    {
        return variableDataOf(static_cast<ST*> (this)) + this->__variableDataSize;
    }

    const_iterator end() const
    {
        return variableDataOf(static_cast<const ST*> (this)) + this->__variableDataSize;
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
    typedef void SuperType;
    typedef ProtoObject SelfType;
    typedef void value_type;

    static constexpr size_t variableDataElementSize = 0;

    virtual NyastObject *getClass() const override;
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
};

struct Behavior : Subclass<Object, Behavior>
{
    virtual Oop lookupSelector(Oop selector) const override;

    Oop superclass;
    Oop methodDict;
};

struct ClassDescription : Subclass<Behavior, ClassDescription>
{

};

struct Class : Subclass<ClassDescription, Class>
{

};

struct Metaclass : Subclass<ClassDescription, Metaclass>
{

};

struct Boolean : Subclass<Object, Boolean>
{
};

struct True : Subclass<Boolean, True>
{
    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<True>::value();
    }
};

struct False : Subclass<Boolean, False>
{
    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<True>::value();
    }
};

struct UndefinedObject : Subclass<Object, UndefinedObject>
{
    static Oop uniqueInstance()
    {
        return ObjectSingletonInstanceOf<True>::value();
    }
};

struct Collection : Subclass<Object, Collection>
{
};

struct SequenceableCollection : Subclass<Collection, SequenceableCollection>
{
};

struct ArrayedCollection : Subclass<SequenceableCollection, ArrayedCollection>
{
};

struct Array : SubclassWithVariableDataOfType<ArrayedCollection, Array, Oop>
{
};

struct String : SubclassWithVariableDataOfType<ArrayedCollection, String, char>
{
    virtual std::string asStdString() const override;
    virtual std::string asString() const override;
    virtual std::string printString() const override;
};

struct Symbol : Subclass<String, Symbol>
{
    virtual std::string printString() const override;
};

struct HashedCollection : Subclass<Collection, HashedCollection>
{
};


struct AbstractDictionary : Subclass<HashedCollection, AbstractDictionary>
{

};

struct Dictionary : Subclass<AbstractDictionary, Dictionary>
{
};

struct MethodDictionary : Subclass<AbstractDictionary, MethodDictionary>
{
    virtual Oop atOrNil(Oop key) const override;

    std::unordered_map<Oop, Oop> elements;
};

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_HPP
