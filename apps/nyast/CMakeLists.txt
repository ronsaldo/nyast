add_executable(nyast-bin nyast.cpp)
target_link_libraries(nyast-bin nyast)
set_target_properties(nyast-bin PROPERTIES OUTPUT_NAME nyast)
