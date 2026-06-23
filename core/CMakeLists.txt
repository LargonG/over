project(core CXX)

include(SetupFMT)
include(SetupGlad)
include(SetupGLFW)
include(SetupGLM)
include(SetupSTB)
include(SetupAssimp)

include(utils/ConfigureSources)

set(p_core_sources
	Camera.cpp
	Shader.cpp
	stb_impl.cpp
	Mesh.cpp
	Model.cpp
	Transform.cpp

	window/Context.cpp
	window/Window.cpp
	
	opengl/VAO.cpp
	opengl/VBO.cpp
	opengl/IBO.cpp
	opengl/Texture2D.cpp
	opengl/Framebuffer.cpp
	opengl/RenderBuffer.cpp

	opengl/allocators/DefaultBufferAllocator.cpp
)

set_source_directory(p_src SOURCE_DIR "src/over/core" SOURCES ${p_core_sources})

add_library(${PROJECT_NAME} STATIC ${p_src})
add_library(over::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC fmt::fmt
	PUBLIC glfw
	PUBLIC glad::glad
	PUBLIC glm::glm
	PUBLIC assimp::assimp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
