include_guard(GLOBAL)

if (USE_VCPKG)
	find_package(glad CONFIG REQUIRED)
else()
	add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad ${CMAKE_BINARY_DIR}/libs/glad)
endif()