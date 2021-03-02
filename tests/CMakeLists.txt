if(NOT WIN32)
	set(EXECUTABLE_OUTPUT_PATH "${Nyast_BINARY_DIR}/dist-tests")
	set(LIBRARY_OUTPUT_PATH "${Nyast_BINARY_DIR}/dist-test")
endif()

add_subdirectory(Nyast)
