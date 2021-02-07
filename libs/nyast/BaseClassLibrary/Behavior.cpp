#include "nyast/BaseClassLibrary/Behavior.hpp"
#include "nyast/BaseClassLibrary/Message.hpp"
#include "nyast/BaseClassLibrary/MethodDictionary.hpp"

namespace nyast
{

MethodBindings Behavior::__instanceMethods__()
{
    return MethodBindings{
        // Runtime required method.
        makeMethodBinding("run:with:in:", +[](Oop self, Oop selector, Oop arguments, Oop receiver) -> Oop {
            return self->runWithIn(selector, arguments->asOopList(), receiver);
        }),

        // Accessing
        makeGetterMethodBinding("superclass", &SelfType::superclass),
        makeSetterMethodBinding("superclass:", &SelfType::superclass),

        makeGetterMethodBinding("instSize", &SelfType::instanceSize),
        makeGetterMethodBinding("instAlignment", &SelfType::instanceAlignment),

        makeGetterMethodBinding("variableDataElementSize", &SelfType::variableDataElementSize),
        makeGetterMethodBinding("variableDataElementAlignment", &SelfType::variableDataElementAlignment),

        makeGetterMethodBinding("classLayout", &SelfType::layout),
        makeSetterMethodBinding("classLayout", &SelfType::layout),

        // Initialization
        makeMethodBinding("initialize", &SelfType::initialize),

        // Testing methods
        makeMethodBinding("isBehavior", &SelfType::isBehavior),

        // Method dictionary
        makeGetterMethodBinding("methodDict", &SelfType::methodDict),
        makeSetterMethodBinding("methodDict:", &SelfType::methodDict),

        makeMethodBinding("lookupSelector:", &SelfType::lookupSelector),
    };
}

bool Behavior::isBehavior() const
{
    return true;
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
    message->lookupClass = self();
    auto messageOop = Oop::fromObjectPtr(message);
    return receiver.perform<Oop> ("doesNotUnderstand:", messageOop);
}

void Behavior::addMethodBindings(const MethodBindings &methods)
{
    for(auto &[selector, method] : methods)
        methodDict->atPut(selector, method);
}

void Behavior::addSlotDefinitions(const SlotDefinitions &slotDefinitions)
{
    (void)slotDefinitions;
}

} // End of namespace nyast
