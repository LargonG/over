include_guard(GLOBAL)

if (USE_VCPKG)
	find_package(glfw3 CONFIG REQUIRED)
else()
	add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw ${CMAKE_BINARY_DIR}/libs/glfw)
endif()
