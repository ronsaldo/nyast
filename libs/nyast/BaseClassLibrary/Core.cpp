#include "nyast/BaseClassLibrary/UndefinedObject.hpp"
#include "nyast/BaseClassLibrary/SmallInteger.hpp"
#include "nyast/BaseClassLibrary/Character.hpp"
#include "nyast/BaseClassLibrary/SmallFloat64.hpp"

namespace nyast
{

const NyastObjectVTable *OopPointerSizeDependentImplementation<uint32_t>::ImmediateVTableTable[ImmediateClassTableSize] = {
    &StaticClassVTableFor<UndefinedObject>::value, // 2r00
    &StaticClassVTableFor<SmallInteger>::value,	 // 2r01
    &StaticClassVTableFor<Character>::value,	 	 // 2r10
    &StaticClassVTableFor<SmallInteger>::value,	 // 2r11
};

const NyastObjectVTable *OopPointerSizeDependentImplementation<uint64_t>::ImmediateVTableTable[ImmediateClassTableSize] = {
    &StaticClassVTableFor<UndefinedObject> ::value, // 2r000
    &StaticClassVTableFor<SmallInteger> ::value,	 // 2r001
    &StaticClassVTableFor<Character> ::value,	 	 // 2r010
    &StaticClassVTableFor<UndefinedObject> ::value, // 2r011 (Reserved)

    &StaticClassVTableFor<SmallFloat64> ::value, 	 // 2r100
    &StaticClassVTableFor<UndefinedObject> ::value, // 2r101 (Reserved)
    &StaticClassVTableFor<UndefinedObject> ::value, // 2r110 (Reserved)
    &StaticClassVTableFor<UndefinedObject> ::value, // 2r111 (Reserved)
};

} // End of namespace nyast
