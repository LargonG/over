project(owlet CXX)

add_subdirectory(libs/glad2)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC)
add_library(over::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 20
)

if(MSVC)
    target_compile_options(
		${PROJECT_NAME}
		PRIVATE /W4 /WX
	)
else()
    target_compile_options(
		${PROJECT_NAME}
		PRIVATE -Wall -Wextra -Wpedantic -Werror
	)
endif()

set(p_core
	debug.cppm
	types.cppm
	owlet.cppm
)
list(TRANSFORM p_core PREPEND "src/lowl/")


set(p_gl_module
	gl.cppm
	parameters.cppm
	context.cppm
	buffer.cppm
	allocator.cppm
	handler.cppm
)
list(TRANSFORM p_gl_module PREPEND "src/lowl/gl/")

set(p_desktop_module
	desktop.cppm
	window.cppm
	monitor.cppm
)
list(TRANSFORM p_desktop_module PREPEND "src/lowl/desktop/")

set(p_hints
	cpp.hint
)
list(TRANSFORM p_hints PREPEND "src/")

target_sources(
	${PROJECT_NAME}
	PUBLIC FILE_SET core TYPE CXX_MODULES FILES ${p_core}
	PUBLIC FILE_SET gl_module TYPE CXX_MODULES FILES ${p_gl_module}
	PUBLIC FILE_SET desktop_module TYPE CXX_MODULES FILES ${p_desktop_module}
	PRIVATE ${p_hints}
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE fmt::fmt
	PUBLIC glad::glad2
	PUBLIC glfw
	PUBLIC glm::glm
	# PRIVATE assimp::assimp

)


add_subdirectory(examples/simple)
