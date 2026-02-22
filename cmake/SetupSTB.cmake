
if (USE_VCPKG)
	find_package(Stb REQUIRED)
else()
	set(Stb_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/stb)
endif()
