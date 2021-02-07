#include "nyast/BaseClassLibrary/ProtoObject.hpp"

namespace nyast
{

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
        makeMethodBinding("class", +[](Oop self) -> Oop {
            return self->getClass();
        }),

        // Comparisons
        makeMethodBinding("identityHash", &SelfType::identityHash),
        makeMethodBinding("==", &SelfType::identityEquals),

        makeMethodBinding("hash", &SelfType::hash),
        makeMethodBinding("=", &SelfType::equals),
    };
}

Oop ProtoObject::yourself()
{
    return self();
}

Oop ProtoObject::doesNotUnderstand(Oop message)
{
    throw std::runtime_error("Message not understood: " + message->printString());
}

MethodBindings ProtoObject::__classMethods__()
{
    return MethodBindings{};
}

void ProtoObject::initialize()
{
    // By default do nothing.
}

size_t ProtoObject::identityHash() const
{
    return std::hash<uintptr_t> ()(reinterpret_cast<uintptr_t> (this));
}

bool ProtoObject::identityEquals(Oop other) const
{
    return self() == other;
}

size_t ProtoObject::hash() const
{
    return identityHash();
}

bool ProtoObject::equals(Oop other) const
{
    return identityEquals(other);
}

Oop ProtoObject::lookupSelector(Oop selector) const
{
    return self().perform<Oop> ("lookupSelector:", selector);
}

Oop ProtoObject::scanFor(Oop key) const
{
    return self().perform<Oop> ("scanFor:", key);
}

void ProtoObject::grow()
{
    self().perform<void> ("grow");
}

Oop ProtoObject::add(Oop value)
{
    return self().perform<Oop> ("add:", value);
}

size_t ProtoObject::getBasicSize() const
{
    return self().perform<size_t> ("basicSize");
}

Oop ProtoObject::basicAt(size_t index) const
{
    return self().perform<Oop> ("basicAt:", index);
}

Oop ProtoObject::basicAtPut(size_t index, Oop value)
{
    return self().perform<Oop> ("basicAt:put:", index, value);
}

size_t ProtoObject::getSize() const
{
    return self().perform<size_t> ("size");
}

Oop ProtoObject::at(Oop key) const
{
    return self().perform<Oop> ("at:", key);
}

Oop ProtoObject::atPut(Oop key, Oop value)
{
    return self().perform<Oop> ("at:put:", key, value);
}

Oop ProtoObject::atOrNil(Oop key) const
{
    return self().perform<Oop> ("atOrNil:", key);
}

Oop ProtoObject::getKey() const
{
    return self().perform<Oop> ("key");
}

Oop ProtoObject::evaluateValue() const
{
    return self().perform<Oop> ("value");
}

Oop ProtoObject::runWithIn(Oop selector, const OopList &arguments, Oop receiver)
{
    return self().perform<Oop> ("run:with:in:", selector, arguments, receiver);
}

bool ProtoObject::isArray() const
{
    return self().perform<bool> ("isArray");
}

bool ProtoObject::isAssociation() const
{
    return self().perform<bool> ("isAssociation");
}

bool ProtoObject::isBehavior() const
{
    return self().perform<bool> ("isBehavior");
}

bool ProtoObject::isBlock() const
{
    return self().perform<bool> ("isBlock");
}

bool ProtoObject::isCharacter() const
{
    return self().perform<bool> ("isCharacter");
}

bool ProtoObject::isDictionary() const
{
    return self().perform<bool> ("isDictionary");
}

bool ProtoObject::isFloat() const
{
    return self().perform<bool> ("isFloat");
}

bool ProtoObject::isFraction() const
{
    return self().perform<bool> ("isFraction");
}

bool ProtoObject::isInteger() const
{
    return self().perform<bool> ("isInteger");
}

bool ProtoObject::isInterval() const
{
    return self().perform<bool> ("isInterval");
}

bool ProtoObject::isNumber() const
{
    return self().perform<bool> ("isNumber");
}

bool ProtoObject::isString() const
{
    return self().perform<bool> ("isString");
}

bool ProtoObject::isSymbol() const
{
    return self().perform<bool> ("isSymbol");
}

MethodLookupResult ProtoObject::asMethodLookupResult(MessageDispatchTrampolineSet trampolineSet) const
{
    return MethodLookupResult{const_cast<ProtoObject*> (this), trampolineSet.objectMethodDispatchTrampoline};
}

std::string ProtoObject::asString() const
{
    auto className = self().getClass()->asString();
    if(!className.empty() && isVowel(className.front()))
        return "an " + className;
    return "a " + className;
}

std::string ProtoObject::printString() const
{
    return self()->asString();
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

} // End of namespace nyast
