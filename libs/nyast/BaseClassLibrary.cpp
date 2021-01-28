#include "nyast/BaseClassLibrary.hpp"
#include <sstream>
#include <iostream>

namespace nyast
{

static std::unordered_map<std::string, Oop> internedSymbols;

NyastObject *OopPointerSizeDependentImplementation<uint32_t>::ImmediateClassTable[ImmediateClassTableSize] = {
    staticClassObjectFor<UndefinedObject> ().asObjectPtr(), // 2r00
    staticClassObjectFor<SmallInteger> ().asObjectPtr(),	 // 2r01
    staticClassObjectFor<Character> ().asObjectPtr(),	 	 // 2r10
    staticClassObjectFor<SmallInteger> ().asObjectPtr(),	 // 2r11
};

NyastObject *OopPointerSizeDependentImplementation<uint64_t>::ImmediateClassTable[ImmediateClassTableSize] = {
    staticClassObjectFor<UndefinedObject> ().asObjectPtr(), // 2r000
    staticClassObjectFor<SmallInteger> ().asObjectPtr(),	 // 2r001
    staticClassObjectFor<Character> ().asObjectPtr(),	 	 // 2r010
    staticClassObjectFor<UndefinedObject> ().asObjectPtr(), // 2r011 (Reserved)

    staticClassObjectFor<SmallFloat64> ().asObjectPtr(), 	 // 2r100
    staticClassObjectFor<UndefinedObject> ().asObjectPtr(), // 2r101 (Reserved)
    staticClassObjectFor<UndefinedObject> ().asObjectPtr(), // 2r110 (Reserved)
    staticClassObjectFor<UndefinedObject> ().asObjectPtr(), // 2r111 (Reserved)
};

Oop Oop::fromInt64(int64_t value)
{
    if(SmallIntegerMinValue <= value && value <= SmallIntegerMaxValue)
        return Oop{(uintptr_t(value) << SmallIntegerTagShift) | SmallIntegerTagValue};

    if(value < 0)
    {
        auto largeInteger = newInstance<LargeNegativeInteger> (8);
        auto absoluteValue = -value;
        memcpy(largeInteger->variableData(), &absoluteValue, 8);
        return Oop::fromObjectPtr(largeInteger);
    }
    else
    {
        auto largeInteger = newInstance<LargePositiveInteger> (8);
        memcpy(largeInteger->variableData(), &value, 8);
        return Oop::fromObjectPtr(largeInteger);
    }
}

Oop Oop::fromUInt64(uint64_t value)
{
    if(value <= uint64_t(SmallIntegerMaxValue))
        return Oop{(uintptr_t(value) << SmallIntegerTagShift) | SmallIntegerTagValue};

    auto largeInteger = newInstance<LargePositiveInteger> (8);
    memcpy(largeInteger->variableData(), &value, 8);
    return Oop::fromObjectPtr(largeInteger);
}

Oop Oop::fromFloat64(double value)
{
    auto boxedFloat64 = newInstance<BoxedFloat64> ();
    boxedFloat64->value = value;
    return Oop::fromObjectPtr(boxedFloat64);
}

Oop Oop::fromByteArray(const ByteArrayData &data)
{
    auto object = newInstance<ByteArray> (data.size());
    if(!data.empty())
        memcpy(object->variableData(), data.data(), data.size());
    return Oop::fromObjectPtr(object);
}

Oop Oop::fromString(const std::string &string)
{
    auto object = newInstance<String> (string.size());
    if(!string.empty())
        memcpy(object->variableData(), string.data(), string.size());
    return Oop::fromObjectPtr(object);
}

Oop Oop::internSymbol(const std::string &string)
{
    auto it = internedSymbols.find(string);
    if(it != internedSymbols.end())
        return it->second;

    auto object = newInstance<Symbol> (string.size());
    if(!string.empty())
        memcpy(object->variableData(), string.data(), string.size());

    auto oop = Oop::fromObjectPtr(object);
    internedSymbols.insert(std::make_pair(string, oop));
    return oop;
}

Oop Oop::fromOopList(const std::vector<Oop> &list)
{
    auto object = newInstance<Array> (list.size());
    auto dest = object->variableData();
    for(size_t i = 0; i < list.size(); ++i)
        dest[i] = list[i];

    return Oop::fromObjectPtr(object);
}

// The important singletons.
Oop::Oop()
    : OopPointerSizeDependentImplementation{UndefinedObject::uniqueInstance().value}
{
}

Oop Oop::nil()
{
    return UndefinedObject::uniqueInstance();
}

Oop Oop::trueValue()
{
    return True::uniqueInstance();
}

Oop Oop::falseValue()
{
    return False::uniqueInstance();
}

//==============================================================================
// ProtoObject
//==============================================================================

static bool isVowel(char c)
{
    switch(c)
    {
    case 'a': return true;
    case 'A': return true;
    case 'e': return true;
    case 'E': return true;
    case 'i': return true;
    case 'I': return true;
    case 'o': return true;
    case 'O': return true;
    case 'u': return true;
    case 'U': return true;
    default: return false;
    }
}

NyastObject::~NyastObject()
{
}

ProtoObject::~ProtoObject()
{
}

MethodBindings ProtoObject::__instanceMethods__()
{
    return MethodBindings{
        // Basic methods.
        makeMethodBinding("doesNotUnderstand:", &SelfType::doesNotUnderstand),
        makeMethodBinding("yourself", &SelfType::yourself),
        makeMethodBinding("initialize", &SelfType::initialize),

        // Comparisons
        makeMethodBinding("identityHash", &SelfType::identityHash),
        makeMethodBinding("==", &SelfType::identityEquals),

        makeMethodBinding("hash", &SelfType::hash),
        makeMethodBinding("=", &SelfType::equals),
    };
}

Oop ProtoObject::yourself()
{
    return asOop();
}

Oop ProtoObject::doesNotUnderstand(Oop message)
{
    throw std::runtime_error("Message not understood: " + message.printString());
}

MethodBindings ProtoObject::__classMethods__()
{
    return MethodBindings{};
}

void ProtoObject::initialize()
{
    // By default do nothing.
}

bool ProtoObject::identityHash() const
{
    return std::hash<uintptr_t> ()(reinterpret_cast<uintptr_t> (this));
}

bool ProtoObject::identityEquals(Oop other) const
{
    return asOop() == other;
}

bool ProtoObject::hash() const
{
    return identityHash();
}

bool ProtoObject::equals(Oop other) const
{
    return identityEquals(other);
}

Oop ProtoObject::lookupSelector(Oop selector) const
{
    return asOop().perform<Oop> ("lookupSelector:", selector);
}

Oop ProtoObject::scanFor(Oop key) const
{
    return asOop().perform<Oop> ("scanFor:", key);
}

void ProtoObject::grow()
{
    asOop().perform<void> ("grow");
}

void ProtoObject::add(Oop value)
{
    asOop().perform<void> ("add:", value);
}

size_t ProtoObject::getBasicSize() const
{
    return asOop().perform<size_t> ("basicSize");
}

Oop ProtoObject::basicAt(size_t index) const
{
    return asOop().perform<Oop> ("basicAt:", index);
}

Oop ProtoObject::basicAtPut(size_t index, Oop value)
{
    return asOop().perform<Oop> ("basicAt:put:", index, value);
}

size_t ProtoObject::getSize() const
{
    return asOop().perform<size_t> ("size");
}

Oop ProtoObject::at(Oop key) const
{
    return asOop().perform<Oop> ("at:", key);
}

Oop ProtoObject::atPut(Oop key, Oop value)
{
    return asOop().perform<Oop> ("at:put:", key, value);
}

Oop ProtoObject::atOrNil(Oop key) const
{
    return asOop().perform<Oop> ("atOrNil:", key);
}

Oop ProtoObject::getKey() const
{
    return asOop().perform<Oop> ("key");
}

Oop ProtoObject::getValue() const
{
    return asOop().perform<Oop> ("value");
}

Oop ProtoObject::runWithIn(Oop selector, const OopList &arguments, Oop self)
{
    return asOop().perform<Oop> ("run:with:in:", selector, arguments, self);
}

MethodLookupResult ProtoObject::asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const
{
    return MethodLookupResult{const_cast<ProtoObject*> (this), trampolineSet.objectMethodDispatchTrampoline};
}

std::string ProtoObject::asString() const
{
    auto className = getClass()->asString();
    if(!className.empty() && isVowel(className.front()))
        return "an " + className;
    return "a " + className;
}

std::string ProtoObject::printString() const
{
    return asString();
}

bool ProtoObject::asBoolean8() const
{
    throw std::runtime_error("Cannot cast object into Boolean8.");
}

uint32_t ProtoObject::asUInt32() const
{
    throw std::runtime_error("Cannot cast object into UInt32.");
}

int32_t ProtoObject::asInt32() const
{
    throw std::runtime_error("Cannot cast object into Int32.");
}

uint64_t ProtoObject::asUInt64() const
{
    throw std::runtime_error("Cannot cast object into UInt64.");
}

int64_t ProtoObject::asInt64() const
{
    throw std::runtime_error("Cannot cast object into Int64.");
}

char32_t ProtoObject::asChar32() const
{
    throw std::runtime_error("Cannot cast object into Char32.");
}

double ProtoObject::asFloat64() const
{
    throw std::runtime_error("Cannot cast object into Float64.");
}

std::string ProtoObject::asStdString() const
{
    throw std::runtime_error("Cannot cast object into std::string.");
}

std::vector<Oop> ProtoObject::asOopList() const
{
    throw std::runtime_error("Cannot cast object into std::vector<Oop>.");
}

ByteArrayData ProtoObject::asByteArrayData() const
{
    throw std::runtime_error("Cannot cast object into ByteArrayData.");
}

//==============================================================================
// NativeMethod
//==============================================================================
MethodLookupResult NativeMethod::asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const
{
    return MethodLookupResult{entryPoint, trampolineSet.nativeMethodDispatchTrampoline};
}

//==============================================================================
// Behavior
//==============================================================================
MethodBindings Behavior::__instanceMethods__()
{
    return MethodBindings{
        makeRawNativeMethodBinding("run:with:in:", +[](Oop self, Oop selector, Oop arguments, Oop receiver) -> Oop {
            return self->runWithIn(selector, arguments.asOopList(), receiver);
        })
    };
}

void Behavior::initialize()
{
    methodDict = Oop::fromObjectPtr(newInstance<MethodDictionary> ());
}

Oop Behavior::lookupSelector(Oop selector) const
{
    auto method = methodDict->atOrNil(selector);
    if(method.isNotNil())
        return method;

    if(!superclass.isNil())
        return superclass->lookupSelector(selector);

    return Oop::nil();
}

Oop Behavior::runWithIn(Oop selector, const OopList &marshalledArguments, Oop receiver)
{
    auto message = basicNewInstance<Message> ();
    message->selector = selector;
    message->args = Oop::fromOopList(marshalledArguments);
    message->lookupClass = asOop();
    auto messageOop = Oop::fromObjectPtr(message);
    return receiver.perform<Oop> ("doesNotUnderstand:", messageOop);
}

void Behavior::addMethodBindings(const MethodBindings &methods)
{
    for(auto &[selector, method] : methods)
        methodDict->atPut(selector, method);
}

//==============================================================================
// Class
//==============================================================================
NyastObject *Class::getClass() const
{
    return metaClass.asObjectPtr();
}

std::string Class::asString() const
{
    return name.asString();
}

//==============================================================================
// Metaclass
//==============================================================================
std::string Metaclass::asString() const
{
    if(thisClass.isNotNil())
        return thisClass.asString() + " class";
    return Super::asString();
}

//==============================================================================
// HashedCollection
//==============================================================================
void HashedCollection::initialize()
{
    tally = 0;
    array = Oop::fromObjectPtr(basicNewInstance<Array> (5));
}

void HashedCollection::fullCheck()
{
    auto capacity = array->getBasicSize();
    if(capacity - tally < std::max(capacity / 4u, size_t(1u)))
        grow();
}

//==============================================================================
// Dictionary
//==============================================================================
Oop Dictionary::atOrNil(Oop key) const
{
    auto elementIndexOop = scanFor(key);
    if(!elementIndexOop.isSmallInteger())
        return Oop::nil();

    auto elementIndex = elementIndexOop.decodeSmallInteger();
    if(elementIndex == 0)
        return Oop::nil();

    return array->basicAt(elementIndex)
        ->getValue();
}

//==============================================================================
// MethodDictionary
//==============================================================================
void MethodDictionary::initialize()
{
    Super::initialize();
    methods = Oop::fromObjectPtr(basicNewInstance<Array> (array->getBasicSize()));
}

Oop MethodDictionary::scanFor(Oop key) const
{
    auto hash = key.identityHash();
    auto finish = array->getBasicSize();
    auto start = (hash % finish) + 1;

    for(size_t i = start; i <= finish; ++i)
    {
        auto element = array->basicAt(i);
        if(element.isNil() || element.identityEquals(key))
            return Oop::fromSize(i);
    }

    for(size_t i = 1; i < start; ++i)
    {
        auto element = array->basicAt(i);
        if(element.isNil() || element.identityEquals(key))
            return Oop::fromSize(i);
    }

    return Oop::fromSize(0);
}

Oop MethodDictionary::atOrNil(Oop key) const
{
    auto elementIndex = scanFor(key).decodeSmallInteger();
    if(elementIndex == 0)
        return Oop::nil();

    return methods->basicAt(elementIndex);
}

Oop MethodDictionary::atPut(Oop key, Oop value)
{
    auto elementIndex = scanFor(key).decodeSmallInteger();
    assert(elementIndex != 0);

    if(array->basicAt(elementIndex).isNil())
    {
        array->basicAtPut(elementIndex, key);
        methods->basicAtPut(elementIndex, value);
        ++tally;
        fullCheck();
    }
    else
    {
        methods->basicAtPut(elementIndex, value);
    }

    return value;
}

void MethodDictionary::grow()
{
    auto oldArray = array;
    auto oldMethods = methods;
    auto oldCapacity = oldArray->getBasicSize();
    auto newCapacity = oldCapacity*2;

    tally = 0;
    array = Oop::fromObjectPtr(basicNewInstance<Array> (newCapacity));
    methods = Oop::fromObjectPtr(basicNewInstance<Array> (newCapacity));

    for(size_t i = 1; i <= oldCapacity; ++i)
    {
        auto selector = oldArray->basicAt(i);
        if(selector.isNotNil())
        {
            atPut(selector, oldMethods->basicAt(i));
        }
    }
}

//==============================================================================
// UndefinedObject
//==============================================================================
std::string UndefinedObject::asString() const
{
    return "nil";
}

//==============================================================================
// True
//==============================================================================
std::string True::asString() const
{
    return "true";
}

//==============================================================================
// False
//==============================================================================
std::string False::asString() const
{
    return "false";
}

//==============================================================================
// Array
//==============================================================================
OopList Array::asOopList() const
{
    return OopList(begin(), end());
}

//==============================================================================
// ByteArray
//==============================================================================

ByteArrayData ByteArray::asByteArrayData() const
{
    return ByteArrayData(begin(), end());
}

std::string ByteArray::asString() const
{
    return printString();
}

std::string ByteArray::printString() const
{
    std::ostringstream out;
    out << "#[";
    auto isFirst = true;
    for(auto &v : *this)
    {
        if(isFirst)
            isFirst = false;
        else
            out << ' ';

        out << int(v);
    }

    out << ']' << std::ends;
    return out.str();
}

//==============================================================================
// String
//==============================================================================
std::string String::asStdString() const
{
    return std::string(begin(), end());
}

std::string String::asString() const
{
    return std::string(begin(), end());
}

std::string String::printString() const
{
    std::string out;
    out.reserve(size() + 2);
    out.push_back('\'');
    for(auto c : *this)
    {
        if(c == '\'')
            out.push_back('\'');
        out.push_back(c);
    }

    out.push_back('\'');
    return out;
}

//==============================================================================
// Symbol
//==============================================================================
std::string Symbol::printString() const
{
    std::string out;
    out.reserve(size() + 3);
    out.push_back('#');
    out.push_back('\'');
    for(auto c : *this)
    {
        if(c == '\'')
            out.push_back('\'');
        out.push_back(c);
    }

    out.push_back('\'');
    return out;
}

//==============================================================================
// Association
//==============================================================================
Oop Association::getKey() const
{
    return key;
}

Oop Association::getValue() const
{
    return value;
}

//==============================================================================
// Message
//==============================================================================

std::string Message::printString() const
{
    std::ostringstream out;
    out << selector.printString();
    for(auto &arg : args.asOopList())
        out << " " << arg.printString();
    out << std::ends;
    return out.str();
}

//==============================================================================
// LargePositiveInteger
//==============================================================================
uint64_t LargePositiveInteger::asUInt64() const
{
    if(size() > sizeof(uint64_t))
        return Super::asUInt64();

    uint64_t decodedValue = 0;
    // FIXME: Support the big endian variant.
    memcpy(&decodedValue, variableData(), size());
    return decodedValue;
}

int64_t LargePositiveInteger::asInt64() const
{
    auto value = asUInt64();
    if(value > uint64_t(INT64_MAX))
        return Super::asInt64();

    return int64_t(value);
}

double LargePositiveInteger::asFloat64() const
{
    // FIXME: Implement this in a much more proper way.
    return double(asUInt64());
}

//==============================================================================
// LargeNegativeInteger
//==============================================================================
int64_t LargeNegativeInteger::asInt64() const
{
    if(size() > sizeof(uint64_t))
        return Super::asInt64();

    uint64_t decodedValue = 0;
    // FIXME: Support the big endian variant.
    memcpy(&decodedValue, variableData(), size());
    if(decodedValue > -uint64_t(INT64_MIN))
        return Super::asInt64();

    return -int64_t(decodedValue);
}

double LargeNegativeInteger::asFloat64() const
{
    // FIXME: Implement this in a much more proper way.
    return double(asInt64());
}

//==============================================================================
// BoxedFloat64
//==============================================================================
std::string BoxedFloat64::asString() const
{
    std::ostringstream out;
    out << value << std::ends;
    return out.str();
}

double BoxedFloat64::asFloat64() const
{
    return value;
}

} // End of namespace nyast
