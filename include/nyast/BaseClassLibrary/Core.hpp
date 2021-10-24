#ifndef NYAST_BASE_CLASS_LIBRARY_CORE_HPP
#define NYAST_BASE_CLASS_LIBRARY_CORE_HPP

#pragma once

#include "../Oop.hpp"

namespace nyast
{
struct ProtoObject;
struct Behavior;
struct ClassDescription;
struct Class;
struct Metaclass;

typedef std::pair<Oop, Oop> MethodBinding;
typedef std::vector<MethodBinding> MethodBindings;

typedef std::pair<std::string, MethodBindings> MethodCategory;
typedef std::vector<MethodCategory> MethodCategories;

typedef Oop SlotDefinition;
typedef OopList SlotDefinitions;

template <typename T>
struct StaticClassVTableFor;

template<typename T>
inline constexpr NyastObjectVTable makeDefaultStaticClassVTable()
{
    return NyastObjectVTable{
        // Construction / finalization
        // basicInitialize.
        +[](AbiOop self) {
            if constexpr (std::is_default_constructible<T>::value)
                new (reinterpret_cast<T*> (self)) T;
            else
                throw std::runtime_error("Cannot basic initialize non-default constructible type.");
        },

        // initialize
        +[](AbiOop self) {
            return reinterpret_cast<T*> (self)->initialize();
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
        +[](AbiOop self, Oop selector, const OopList& marshalledArguments, Oop receiver) -> Oop {
            return reinterpret_cast<T*> (self)->runWithIn(selector, marshalledArguments, receiver);
        },

        // asMethodLookupResultFor:
        +[](AbiOop self, MessageDispatchTrampolineSet trampolineSet) -> MethodLookupResult {
            return reinterpret_cast<T*> (self)->asMethodLookupResult(trampolineSet);
        },

        // addSubclass:
        +[](AbiOop self, Oop subclass) -> void {
            return reinterpret_cast<T*> (self)->addSubclass(subclass);
        },

        // basicNew
        +[](AbiOop self) -> Oop {
            return reinterpret_cast<T*> (self)->basicNewInstance();
        },

        // basicNew:
        +[](AbiOop self, size_t variableDataSize) -> Oop {
            return reinterpret_cast<T*> (self)->basicNewInstance(variableDataSize);
        },

        // new
        +[](AbiOop self) -> Oop {
            return reinterpret_cast<T*> (self)->newInstance();
        },

        // new:
        +[](AbiOop self, size_t variableDataSize) -> Oop {
            return reinterpret_cast<T*> (self)->newInstance(variableDataSize);
        },

        // classLayout
        +[](AbiOop self) -> Oop {
            return reinterpret_cast<T*> (self)->getClassLayout();
        },

        // slotScope
        +[](AbiOop self) -> Oop {
            return reinterpret_cast<T*> (self)->getSlotScope();
        },

        // gcLayout
        +[](AbiOop self) -> Oop {
            return reinterpret_cast<T*> (self)->getGCLayout();
        },

        // GCLayout
        //atOffset:size:setReferenceType:
        +[](AbiOop self, size_t offset, size_t valueSize, GCReferenceType value) -> void {
            return reinterpret_cast<T*> (self)->setGCReferenceTypeAtOffsetSize(offset, valueSize, value);
        },

        // Slot
        // name
        +[](AbiOop self) -> Oop {
            return reinterpret_cast<T*> (self)->getName();
        },

        // read:
        +[](AbiOop self, Oop receiver) -> Oop {
            return reinterpret_cast<T*> (self)->read(receiver);
        },

        // write:to:
        +[](AbiOop self, Oop value, Oop receiver) -> Oop {
            return reinterpret_cast<T*> (self)->writeTo(value, receiver);
        },

        // storeReferenceTypesInGCLayout:
        +[](AbiOop self, Oop gcLayout) -> void {
            return reinterpret_cast<T*> (self)->storeReferenceTypesInGCLayout(gcLayout);
        },

        // Basic operations
        // identityHash
        +[](AbiOop self) -> OopHash {
            return reinterpret_cast<T*> (self)->identityHash();
        },

        // ==
        +[](AbiOop self, Oop other) -> bool {
            return reinterpret_cast<T*> (self)->identityEquals(other);
        },

        // hash
        +[](AbiOop self) -> OopHash {
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
        // value:
        +[](AbiOop self, Oop arg) -> Oop {
            return reinterpret_cast<T*> (self)->evaluateValueWithArg(arg);
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

template<typename T, typename... Args>
T *staticBasicNewInstance(size_t variableDataSize = 0, Args&&... args)
{
    size_t allocationSize = sizeof(T) + T::__variableDataElementSize__ * variableDataSize;

    return reinterpret_cast<T*> (allocateAndInitializeObjectMemoryWith(allocationSize, [&](uint8_t *allocation) {
        memset(allocation, 0, allocationSize);

        auto result = reinterpret_cast<T*> (allocation);

        // Basic initialize the object.
        new (result) T(std::forward<Args> (args)...);

        // Set the object vtable.
        result->__vtable = &StaticClassVTableFor<T>::value;

        // Initialize the variable data.
        result->__variableDataSize = uint32_t(variableDataSize);
        if constexpr(T::__variableDataElementSize__ > 0)
            new (variableDataOf(result)) typename T::value_type[variableDataSize];
    }));
}

template<typename T, typename... Args>
T *staticNewInstance(size_t variableDataSize = 0, Args&&... args)
{
    auto result = staticBasicNewInstance<T> (variableDataSize, std::forward<Args> (args)...);
    result->initialize();
    return result;
}

template<typename T, typename... Args>
Oop staticOopBasicNewInstance(size_t variableDataSize = 0, Args&&... args)
{
    return Oop::fromObjectPtr(staticBasicNewInstance<T> (variableDataSize, std::forward<Args> (args)...));
}

template<typename T, typename... Args>
Oop staticOopNewInstance(size_t variableDataSize = 0, Args&&... args)
{
    return Oop::fromObjectPtr(staticNewInstance<T> (variableDataSize, std::forward<Args> (args)...));
}

template <typename T>
struct ObjectSingletonInstanceOf
{
    static inline RootOop rootOop;

    static Oop value()
    {
        if(rootOop.isNull())
            rootOop = staticOopBasicNewInstance<T> ();
        return rootOop.asOop();
    }
};

template <typename T>
struct StaticClassObjectFor
{
    static inline RootOop oop;

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
Oop staticClassObjectFor ()
{
    return StaticClassObjectFor<T>::value();
}

template<typename BT, typename ST>
struct Subclass : BT
{
    typedef BT Super;
    typedef ST SelfType;
    typedef Subclass<BT, ST> DirectSuper;

    using Super::Super;

    static constexpr bool __isImmediate__ = false;

    static Oop __class__()
    {
        return StaticClassObjectFor<SelfType>::value();
    }

    Oop getClass() const
    {
        return __class__();
    }

    static MethodCategories __instanceMethods__()
    {
        return MethodCategories{};
    }

    static SlotDefinitions __slots__()
    {
        return SlotDefinitions{};
    }

    static MethodCategories __classMethods__()
    {
        return MethodCategories{};
    }

    static SlotDefinitions __classSlots__()
    {
        return SlotDefinitions{};
    }
};

template<typename ST, typename SelfType, typename ET>
struct SubclassWithVariableDataOfType : Subclass<ST, SelfType>
{
    typedef ET value_type;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef SubclassWithVariableDataOfType<ST, SelfType, ET> DirectSuper;

    using Subclass<ST, SelfType>::Subclass;

    static constexpr size_t __variableDataElementSize__ = sizeof(value_type);
    static constexpr size_t __variableDataElementAlignment__ = alignof(value_type);
    static constexpr GCReferenceType __variableDataGCReferenceType__ = GCReferenceTypeForFieldType<value_type>::value;

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
    typedef SubclassWithImmediateRepresentation<ST, SelfType> DirectSuper;

    using Subclass<ST, SelfType>::Subclass;

    static constexpr bool __isImmediate__ = true;
};

struct NYAST_CORE_EXPORT ProtoObject : Subclass<NyastObject, ProtoObject>
{
    typedef void Super;
    typedef void value_type;
    typedef Class Meta;

    static constexpr size_t __variableDataElementSize__ = 0;
    static constexpr size_t __variableDataElementAlignment__ = 0;
    static constexpr GCReferenceType __variableDataGCReferenceType__ = GCReferenceType::Value;

    static constexpr char const __className__[] = "ProtoObject";

    static MethodCategories __instanceMethods__();
    static MethodCategories __classMethods__();

    ~ProtoObject();
    Oop initialize();

    Oop lookupSelector(Oop selector) const;
    Oop runWithIn(Oop selector, const OopList &marshalledArguments, Oop self);
    MethodLookupResult asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const;
    void addSubclass(Oop subclass);
    Oop basicNewInstance() const;
    Oop basicNewInstance(size_t variableDataSize) const;
    Oop newInstance() const;
    Oop newInstance(size_t variableDataSize) const;

    Oop getClassLayout();
    Oop getSlotScope() const;
    Oop getGCLayout();
    
    void setGCReferenceTypeAtOffsetSize(size_t offset, size_t valueSize, GCReferenceType value);

    Oop getName() const;
    Oop read(Oop receiver);
    Oop writeTo(Oop value, Oop receiver);
    void storeReferenceTypesInGCLayout(Oop gcLayout);

    OopHash identityHash() const;
    bool identityEquals(Oop other) const;
    OopHash hash() const;
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
    Oop evaluateValueWithArg(Oop arg) const;

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

private:
    Oop yourself();
    Oop doesNotUnderstand(Oop message);
};

struct NYAST_CORE_EXPORT Object : Subclass<ProtoObject, Object>
{
    static constexpr char const __className__[] = "Object";

    static MethodCategories __instanceMethods__();

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

    // Errors.
    Oop error();
    Oop error(const std::string &errorMessage);
    Oop explicitRequirement();
    Oop subclassResponsibility();
    Oop shouldBeImplemented();
    Oop shouldNotImplement();
};

struct NYAST_CORE_EXPORT Behavior : Subclass<Object, Behavior>
{
    static constexpr char const __className__[] = "Behavior";
    static SlotDefinitions __slots__();
    static MethodCategories __instanceMethods__();

    bool isBehavior() const;
    bool hasOopVariableData() const;

    Oop basicNewInstance() const;
    Oop basicNewInstance(size_t variableDataSize) const;
    Oop newInstance() const;
    Oop newInstance(size_t variableDataSize) const;

    Oop initialize();
    Oop lookupSelector(Oop selector) const;
    Oop runWithIn(Oop selector, const OopList &marshalledArguments, Oop self);

    void addMethodCategories(const MethodCategories &methods);
    void setSlotDefinitions(const SlotDefinitions &slotDefinitions);
    void fixupParentSlotScope();
    Oop getClassLayout();
    Oop getGCLayout();

    MemberOop superclass;
    MemberOop methodDict;
    MemberOop layout;
    MemberOop gcLayout;

    const NyastObjectVTable *instanceVTable;

    size_t instanceSize;
    size_t instanceAlignment;

    size_t variableDataElementSize;
    size_t variableDataElementAlignment;
    GCReferenceType variableDataGCReferenceType;
};

struct NYAST_CORE_EXPORT ClassDescription : Subclass<Behavior, ClassDescription>
{
    static constexpr char const __className__[] = "ClassDescription";
};

struct NYAST_CORE_EXPORT Class : Subclass<ClassDescription, Class>
{
    static constexpr char const __className__[] = "Class";

    static MethodCategories __instanceMethods__();
    static SlotDefinitions __slots__();

    std::string asString() const;
    Oop getClass() const;

    Oop getSubclasses();
    void addSubclass(Oop subclass);

    MemberOop subclasses;
    MemberOop name;
    MemberOop metaClass;
};

struct NYAST_CORE_EXPORT Metaclass : Subclass<ClassDescription, Metaclass>
{
    static constexpr char const __className__[] = "Metaclass";

    static MethodCategories __instanceMethods__();
    static SlotDefinitions __slots__();

    std::string asString() const;

    MemberOop thisClass;
};

template <typename T>
struct StaticClassVTableFor
{
    static inline const auto value = makeDefaultStaticClassVTable<T> ();
};

template <typename T>
Oop StaticClassObjectFor<T>::value()
{
    if(oop.isNotNilOrNull())
        return oop;

    typedef T InstanceSide;
    typedef typename T::Meta ClassSide;

    auto metaClass = staticNewInstance<Metaclass> ();
    auto clazz = staticNewInstance<ClassSide> ();
    clazz->metaClass = Oop::fromObjectPtr(metaClass);
    metaClass->thisClass = Oop::fromObjectPtr(clazz);
    oop = Oop::fromObjectPtr(clazz);

    Oop superClass = staticClassObjectFor<typename T::Super> ();
    clazz->superclass = superClass;

    if(superClass.isNotNil())
    {
        metaClass->superclass = superClass.getClass();
        superClass->addSubclass(oop);
    }
    else
    {
        metaClass->superclass = staticClassObjectFor<Class> (); // Short circuit
    }

    metaClass->instanceVTable = &StaticClassVTableFor<Metaclass>::value;
    clazz->instanceVTable = &StaticClassVTableFor<InstanceSide>::value;
    clazz->instanceSize = InstanceSide::__isImmediate__ ? 0 : sizeof(InstanceSide);
    clazz->instanceAlignment = InstanceSide::__isImmediate__ ? 1 : alignof(InstanceSide);

    clazz->variableDataElementSize = InstanceSide::__variableDataElementSize__;
    clazz->variableDataElementAlignment = InstanceSide::__variableDataElementAlignment__;
    clazz->variableDataGCReferenceType = InstanceSide::__variableDataGCReferenceType__;

    metaClass->instanceSize = ClassSide::__isImmediate__ ? 0 : sizeof(ClassSide);
    metaClass->instanceAlignment = ClassSide::__isImmediate__ ? 1 : alignof(ClassSide);

    metaClass->variableDataElementSize = ClassSide::__variableDataElementSize__;
    metaClass->variableDataElementAlignment = ClassSide::__variableDataElementAlignment__;
    metaClass->variableDataGCReferenceType = ClassSide::__variableDataGCReferenceType__;

    clazz->name = Oop::internSymbol(T::__className__);
    clazz->addMethodCategories(T::__instanceMethods__());
    clazz->setSlotDefinitions(T::__slots__());
    clazz->fixupParentSlotScope();

    metaClass->addMethodCategories(T::__classMethods__());
    metaClass->setSlotDefinitions(T::__classSlots__());
    metaClass->fixupParentSlotScope();

    return oop;
}

} // End of namespace nyast

#endif //NYAST_BASE_CLASS_LIBRARY_CORE_HPP
