include_guard(GLOBAL)


if (USE_VCPKG)
	find_package(assimp CONFIG REQUIRED)
else()
	# suffer #2
	include(InitSubmodules)

	init_submodule(libs/assimp)

	add_subdirectory(${CMAKE_SOURCE_DIR}/libs/assimp ${CMAKE_BINARY_DIR}/libs/assimp)
endif()
