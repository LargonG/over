cmake_minimum_required (VERSION 3.20..3.31)

set(CMAKE_CXX_STANDARD 17)

if(POLICY CMP0140)
	cmake_policy(SET CMP0140 NEW)
else()
	message(FATAL_ERROR "Nah, we don't support it, install newer cmake version'")
endif()

if (POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

project(over CXX)

option(USE_VCPKG "Use vcpkg package manager" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(basics)
add_subdirectory(fractal)
add_subdirectory(lighting)
add_subdirectory(models)
add_subdirectory(morphing)
add_subdirectory(features)
add_subdirectory(atmosphere)

add_subdirectory(owlet)
