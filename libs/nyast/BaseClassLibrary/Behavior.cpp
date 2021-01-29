#include "nyast/BaseClassLibrary/Behavior.hpp"
#include "nyast/BaseClassLibrary/Message.hpp"
#include "nyast/BaseClassLibrary/MethodDictionary.hpp"

namespace nyast
{

MethodBindings Behavior::__instanceMethods__()
{
    return MethodBindings{
        makeRawNativeMethodBinding("run:with:in:", +[](Oop self, Oop selector, Oop arguments, Oop receiver) -> Oop {
            return self->runWithIn(selector, arguments->asOopList(), receiver);
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
    message->lookupClass = self();
    auto messageOop = Oop::fromObjectPtr(message);
    return receiver.perform<Oop> ("doesNotUnderstand:", messageOop);
}

void Behavior::addMethodBindings(const MethodBindings &methods)
{
    for(auto &[selector, method] : methods)
        methodDict->atPut(selector, method);
}

} // End of namespace nyast
