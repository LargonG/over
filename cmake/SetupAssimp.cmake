
if (USE_VCPKG)
	find_package(assimp CONFIG REQUIRED)
else()
	# suffer #2
	add_subdirectory(${CMAKE_SOURCE_DIR}/libs/assimp ${CMAKE_BINARY_DIR}/libs/assimp)
endif()
