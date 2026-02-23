
if (USE_VCPKG)
	find_package(Stb REQUIRED)
else()
	include(InitSubmodules)

	init_submodule(libs/stb)
	
	set(Stb_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/stb)
endif()
