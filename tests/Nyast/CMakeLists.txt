set(NyastTests_Sources
    Main.cpp
    Object.cpp
)

add_executable(NyastTests ${NyastTests_Sources})
target_link_libraries(NyastTests UnitTest++ nyast)

add_test(NAME RunNyastTests COMMAND NyastTests)
