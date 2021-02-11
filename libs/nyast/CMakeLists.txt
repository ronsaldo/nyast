set(Nyast_Sources
    Oop.cpp

    BaseClassLibrary/Array.cpp
    BaseClassLibrary/Association.cpp
    BaseClassLibrary/Behavior.cpp
    BaseClassLibrary/BoxedFloat64.cpp
    BaseClassLibrary/ByteArray.cpp
    BaseClassLibrary/Character.cpp
    BaseClassLibrary/Class.cpp
    BaseClassLibrary/Core.cpp
    BaseClassLibrary/Dictionary.cpp
    BaseClassLibrary/False.cpp
    BaseClassLibrary/Fraction.cpp
    BaseClassLibrary/HashedCollection.cpp
    BaseClassLibrary/LargeInteger.cpp
    BaseClassLibrary/LargeNegativeInteger.cpp
    BaseClassLibrary/LargePositiveInteger.cpp
    BaseClassLibrary/Message.cpp
    BaseClassLibrary/Metaclass.cpp
    BaseClassLibrary/MethodDictionary.cpp
    BaseClassLibrary/NativeClassRegistry.cpp
    BaseClassLibrary/NativeMethod.cpp
    BaseClassLibrary/Number.cpp
    BaseClassLibrary/Object.cpp
    BaseClassLibrary/ProtoObject.cpp
    BaseClassLibrary/Slot.cpp
    BaseClassLibrary/SmallFloat64.cpp
    BaseClassLibrary/SmallInteger.cpp
    BaseClassLibrary/String.cpp
    BaseClassLibrary/Symbol.cpp
    BaseClassLibrary/True.cpp
    BaseClassLibrary/UndefinedObject.cpp
    BaseClassLibrary/Variable.cpp
)

set(Nyast_SmalltalkCompiler_Sources
    SmalltalkCompiler/Scanner.cpp
    SmalltalkCompiler/Parser.cpp
    SmalltalkCompiler/Evaluator.cpp
)

add_library(nyast SHARED ${Nyast_Sources} ${Nyast_SmalltalkCompiler_Sources})
