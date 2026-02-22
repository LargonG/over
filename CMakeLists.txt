cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

project(over CXX)

option(USE_VCPKG "Use vcpkg package manager" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(SetupFMT)
include(SetupGlad)
include(SetupGLFW)
include(SetupGLM)
include(SetupSTB)
include(SetupImGui)
include(SetupAssimp)

add_subdirectory(test)

add_subdirectory(core)
add_subdirectory(basics)
add_subdirectory(fractal)
add_subdirectory(lighting)
add_subdirectory(models)
