#include "nyast/Oop.hpp"
#include <sstream>

namespace nyast
{
Oop Oop::performInSuperclassWithArguments(InlineCache *inlineCache, Oop clazz, Oop selector, const OopList &arguments)
{
    switch(arguments.size())
    {
    case 0: return performInSuperclass<Oop> (inlineCache, clazz, selector);
    case 1: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0]);
    case 2: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1]);
    case 3: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1], arguments[2]);
    case 4: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1], arguments[2], arguments[3]);
    case 5: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4]);
    case 6: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5]);
    case 7: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6]);
    case 8: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7]);
    case 9: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7], arguments[8]);
    case 10: return performInSuperclass<Oop> (inlineCache, clazz, selector, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7], arguments[8], arguments[9]);
    default:
        throw std::logic_error("Unsupported perform with many arguments.");
    }
}

const char *GCReferenceTypeToString(GCReferenceType referenceType)
{
    switch(referenceType)
    {
    case GCReferenceType::Value: return "value";
    case GCReferenceType::Strong: return "strong";
    case GCReferenceType::Weak: return "strong";
    default: abort();
    }
}

} // End of namespace nyast
