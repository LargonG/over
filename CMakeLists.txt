# CMakeList.txt : CMake project for over, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("over")

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)


# Add source to this project's executable.
add_executable (over "main.cpp" "actor.cpp" "debug_system.cpp")

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/vertex.shader
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/fragment.shader
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET over PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(over PRIVATE fmt::fmt)
target_link_libraries(over PRIVATE glfw)
target_link_libraries(over PRIVATE glad::glad)