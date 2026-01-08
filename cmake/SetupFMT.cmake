
if (USE_VCPKG) 
	find_package(fmt CONFIG REQUIRED)
else()
	add_subdirectory(${CMAKE_SOURCE_DIR}/libs/fmt ${CMAKE_BINARY_DIR}/libs/fmt)
endif()
