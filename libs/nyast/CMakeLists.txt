set(Nyast_Sources
    Object.cpp
    BaseClassLibrary.cpp
)

set(Nyast_SmalltalkCompiler_Sources
    SmalltalkCompiler/Scanner.cpp
    SmalltalkCompiler/Parser.cpp
    SmalltalkCompiler/Evaluator.cpp
)

add_library(nyast STATIC ${Nyast_Sources} ${Nyast_SmalltalkCompiler_Sources})
