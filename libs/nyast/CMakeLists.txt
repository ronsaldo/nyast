set(Nyast_Sources
    Object.cpp
    BaseClassLibrary.cpp
    Scanner.cpp
    Parser.cpp
)

add_library(nyast STATIC ${Nyast_Sources})
