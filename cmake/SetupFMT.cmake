include_guard(GLOBAL)

if (USE_VCPKG) 
	find_package(fmt CONFIG REQUIRED)
else()
	message(STATUS "FMT CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
	add_subdirectory(${CMAKE_SOURCE_DIR}/libs/fmt ${CMAKE_BINARY_DIR}/libs/fmt)
endif()
