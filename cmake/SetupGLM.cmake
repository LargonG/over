
if (USE_VCPKG)
	find_package(glm CONFIG REQUIRED)
else()
	add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glm ${CMAKE_BINARY_DIR}/libs/glm)
endif()
