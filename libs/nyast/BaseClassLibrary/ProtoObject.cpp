#include "nyast/BaseClassLibrary/ProtoObject.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"

namespace nyast
{
static NativeClassRegistration<ProtoObject> protoObjectClassRegistration;

ProtoObject::~ProtoObject()
{
}

MethodCategories ProtoObject::__instanceMethods__()
{
    return MethodCategories{
        {"basic methods", {
            makeMethodBinding("doesNotUnderstand:", &SelfType::doesNotUnderstand),
            makeMethodBinding("yourself", &SelfType::yourself),
            makeMethodBinding("initialize", &SelfType::initialize),
            makeMethodBinding("class", +[](Oop self) -> Oop {
                return self->getClass();
            }),
        }},

        {"comparisons", {
            makeMethodBinding("identityHash", &SelfType::identityHash),
            makeMethodBinding("==", &SelfType::identityEquals),
            makeMethodBinding("~~", +[](Oop self, Oop other) -> bool {
                return !self->identityEquals(other);
            }),

            makeMethodBinding("hash", &SelfType::hash),
            makeMethodBinding("=", &SelfType::equals),
            makeMethodBinding("~=", +[](Oop self, Oop other) -> bool {
                return !self->equals(other);
            }),
        }},
    };
}

MethodCategories ProtoObject::__classMethods__()
{
    return MethodCategories{};
}

Oop ProtoObject::yourself()
{
    return self();
}

Oop ProtoObject::doesNotUnderstand(Oop message)
{
    throw std::runtime_error("Message not understood: " + message->printString());
}

Oop ProtoObject::initialize()
{
    return self();
}

OopHash ProtoObject::identityHash() const
{
    return hashMultiply(reinterpret_cast<OopHash> (this));
}

bool ProtoObject::identityEquals(Oop other) const
{
    return self() == other;
}

OopHash ProtoObject::hash() const
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

Oop ProtoObject::evaluateValueWithArg(Oop arg)
{
    return self().perform<Oop> ("value:", arg);
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

void ProtoObject::addSubclass(Oop subclass)
{
    self().perform<void> ("addSubclass:", subclass);
}

Oop ProtoObject::basicNewInstance() const
{
    return self().perform<Oop> ("basicNew");
}

Oop ProtoObject::basicNewInstance(size_t variableDataSize) const
{
    return self().perform<Oop> ("basicNew:", variableDataSize);
}

Oop ProtoObject::newInstance() const
{
    return self().perform<Oop> ("new");
}

Oop ProtoObject::newInstance(size_t variableDataSize) const
{
    return self().perform<Oop> ("new:", variableDataSize);
}

Oop ProtoObject::getClassLayout()
{
    return self().perform<Oop> ("classLayout");
}

Oop ProtoObject::getSlotScope() const
{
    return self().perform<Oop> ("slotScope");
}

Oop ProtoObject::getGCLayout()
{
    return self().perform<Oop> ("gcLayout");
}

void ProtoObject::setGCReferenceTypeAtOffsetSize(size_t offset, size_t valueSize, GCReferenceType value)
{
    return self().perform<void> ("atOffset:size:setReferenceType:", offset, valueSize, value);
}

Oop ProtoObject::getName() const
{
    return self().perform<Oop> ("name");
}

Oop ProtoObject::read(Oop receiver)
{
    return self().perform<Oop> ("read:", receiver);
}

Oop ProtoObject::writeTo(Oop value, Oop receiver)
{
    return self().perform<Oop> ("write:to:", receiver, value);
}

void ProtoObject::storeReferenceTypesInGCLayout(Oop gcLayout)
{
    return self().perform<void> ("storeReferenceTypesInGCLayout:", gcLayout);
}

std::string ProtoObject::asString() const
{
    return self()->asString();
}

std::string ProtoObject::printString() const
{
    return self()->printString();
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
