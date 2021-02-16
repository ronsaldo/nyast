#include "nyast/BaseClassLibrary/Behavior.hpp"
#include "nyast/BaseClassLibrary/Message.hpp"
#include "nyast/BaseClassLibrary/MethodDictionary.hpp"

#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"
#include "nyast/BaseClassLibrary/CppMethodBinding.hpp"
#include "nyast/BaseClassLibrary/CppMemberSlot.hpp"
#include <iostream>

namespace nyast
{

static NativeClassRegistration<Behavior> behaviorClassRegistration;

SlotDefinitions Behavior::__slots__()
{
    return SlotDefinitions{
        makeMemberSlot("superclass", &SelfType::superclass),
        makeMemberSlot("methodDict", &SelfType::methodDict),
        makeMemberSlot("layout", &SelfType::layout),

        makeMemberSlot("instanceSize", &SelfType::instanceSize),
        makeMemberSlot("instanceAlignment", &SelfType::instanceAlignment),

        makeMemberSlot("variableDataElementSize", &SelfType::variableDataElementSize),
        makeMemberSlot("variableDataElementAlignment", &SelfType::variableDataElementAlignment),
    };
}

MethodCategories Behavior::__instanceMethods__()
{
    return MethodCategories{
        {"runtime", {
            makeMethodBinding("run:with:in:", +[](Oop self, Oop selector, Oop arguments, Oop receiver) -> Oop {
                return self->runWithIn(selector, arguments->asOopList(), receiver);
            }),
        }},

        {"accessing", {
            makeGetterMethodBinding("superclass", &SelfType::superclass),
            makeSetterMethodBinding("superclass:", &SelfType::superclass),

            makeGetterMethodBinding("instSize", &SelfType::instanceSize),
            makeGetterMethodBinding("instAlignment", &SelfType::instanceAlignment),

            makeGetterMethodBinding("variableDataElementSize", &SelfType::variableDataElementSize),
            makeGetterMethodBinding("variableDataElementAlignment", &SelfType::variableDataElementAlignment),

            makeGetterMethodBinding("classLayout", &SelfType::layout),
            makeSetterMethodBinding("classLayout:", &SelfType::layout),
        }},

        {"instance creation", {
            makeMethodBinding("basicNew", static_cast<Oop(SelfType::*)() const> (&SelfType::basicNewInstance)),
            makeMethodBinding("basicNew:", static_cast<Oop(SelfType::*)(size_t) const> (&SelfType::basicNewInstance)),
            makeMethodBinding("new", static_cast<Oop(SelfType::*)() const> (&SelfType::newInstance)),
            makeMethodBinding("new:", static_cast<Oop(SelfType::*)(size_t) const> (&SelfType::newInstance)),
        }},

        {"initialization", {
            makeMethodBinding("initialize", &SelfType::initialize),
        }},

        {"testing", {
            makeMethodBinding("isBehavior", &SelfType::isBehavior),
        }},

        {"method dictionary", {
            makeGetterMethodBinding("methodDict", &SelfType::methodDict),
            makeSetterMethodBinding("methodDict:", &SelfType::methodDict),

            makeMethodBinding("lookupSelector:", &SelfType::lookupSelector),
        }},
    };
}

Oop Behavior::basicNewInstance() const
{
    return self()->basicNewInstance(0);
}

Oop Behavior::basicNewInstance(size_t variableDataSize) const
{
    size_t allocationSize = instanceSize + variableDataElementSize * variableDataSize;
    char *allocation = new char[allocationSize] ();
    memset(allocation, 0, allocationSize);

    auto result = reinterpret_cast<ProtoObject*> (allocation);
    result->__vtable = instanceVTable;

    // Basic initialize the object.
    instanceVTable->basicInitialize(reinterpret_cast<AbiOop> (result));
    result->__vtable = instanceVTable;

    // Initialize the variable oop data into nil.
    result->__variableDataSize = uint32_t(variableDataSize);
    if(isVariableDataOop)
    {
        assert(variableDataElementSize == sizeof(Oop));
        auto oopData = reinterpret_cast<Oop*> (allocation + instanceSize);
        for(size_t i = 0; i < variableDataSize; ++i)
            oopData[i] = Oop();
    }

    return Oop::fromObjectPtr(result);
}

Oop Behavior::newInstance() const
{
    auto result = self()->basicNewInstance();
    result->initialize();
    return result;
}

Oop Behavior::newInstance(size_t variableDataSize) const
{
    return self()->basicNewInstance(variableDataSize)->initialize();
}

bool Behavior::isBehavior() const
{
    return true;
}

Oop Behavior::initialize()
{
    methodDict = Oop::fromObjectPtr(staticNewInstance<MethodDictionary> ());
    return self();
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
    auto message = staticBasicNewInstance<Message> ();
    message->selector = selector;
    message->args = Oop::fromOopList(marshalledArguments);
    message->lookupClass = self();
    auto messageOop = Oop::fromObjectPtr(message);
    return receiver.perform<Oop> ("doesNotUnderstand:", messageOop);
}

void Behavior::addMethodCategories(const MethodCategories &categories)
{
    for(auto &[category, methods] : categories)
    {
        for(auto &[selector, method] : methods)
            methodDict->atPut(selector, method);        
    }
}

void Behavior::setSlotDefinitions(const SlotDefinitions &slotDefinitions)
{
    (void)slotDefinitions;
}

} // End of namespace nyast
