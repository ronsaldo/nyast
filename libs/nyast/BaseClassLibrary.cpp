#include "nyast/BaseClassLibrary.hpp"
#include <sstream>

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
        auto largeInteger = makeInstance<LargeNegativeInteger> (8);
        auto absoluteValue = -value;
        memcpy(largeInteger->variableData(), &absoluteValue, 8);
        return Oop::fromObjectPtr(largeInteger);
    }
    else
    {
        auto largeInteger = makeInstance<LargePositiveInteger> (8);
        memcpy(largeInteger->variableData(), &value, 8);
        return Oop::fromObjectPtr(largeInteger);
    }
}

Oop Oop::fromUInt64(uint64_t value)
{
    if(value <= uint64_t(SmallIntegerMaxValue))
        return Oop{(uintptr_t(value) << SmallIntegerTagShift) | SmallIntegerTagValue};

    auto largeInteger = makeInstance<LargePositiveInteger> (8);
    memcpy(largeInteger->variableData(), &value, 8);
    return Oop::fromObjectPtr(largeInteger);
}

Oop Oop::fromFloat64(double value)
{
    auto boxedFloat64 = makeInstance<BoxedFloat64> ();
    boxedFloat64->value = value;
    return Oop::fromObjectPtr(boxedFloat64);
}

Oop Oop::fromByteArray(const ByteArrayData &data)
{
    auto object = makeInstance<ByteArray> (data.size());
    if(!data.empty())
        memcpy(object->variableData(), data.data(), data.size());
    return Oop::fromObjectPtr(object);
}

Oop Oop::fromString(const std::string &string)
{
    auto object = makeInstance<String> (string.size());
    if(!string.empty())
        memcpy(object->variableData(), string.data(), string.size());
    return Oop::fromObjectPtr(object);
}

Oop Oop::internSymbol(const std::string &string)
{
    auto it = internedSymbols.find(string);
    if(it != internedSymbols.end())
        return it->second;

    auto object = makeInstance<Symbol> (string.size());
    if(!string.empty())
        memcpy(object->variableData(), string.data(), string.size());

    auto oop = Oop::fromObjectPtr(object);
    internedSymbols.insert(std::make_pair(string, oop));
    return oop;
}

Oop Oop::fromOopList(const std::vector<Oop> &list)
{
    auto object = makeInstance<Array> (list.size());
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

Oop ProtoObject::lookupSelector(Oop selector) const
{
    return asOop().perform<Oop> ("lookupSelector:", selector);
}

Oop ProtoObject::atOrNil(Oop key) const
{
    return asOop().perform<Oop> ("atOrNil:", key);
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
// Behavior
//==============================================================================
Oop Behavior::lookupSelector(Oop selector) const
{
    (void)selector;

    if(!superclass.isNil())
        return superclass.asObjectPtr()->lookupSelector(selector);

    return Oop::nil();
}

//==============================================================================
// Class
//==============================================================================
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
// MethodDictionary
//==============================================================================
Oop MethodDictionary::atOrNil(Oop key) const
{
    auto it = elements.find(key);
    if(it == elements.end())
        return it->second;
    return Oop::nil();
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
    return asUInt64();
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
    if(decodedValue > uint64_t(-INT64_MIN))
        return Super::asInt64();

    return -int64_t(decodedValue);
}

double LargeNegativeInteger::asFloat64() const
{
    // FIXME: Implement this in a much more proper way.
    return asInt64();
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
