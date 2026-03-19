project(core CXX)

include(SetupFMT)
include(SetupGlad)
include(SetupGLFW)
include(SetupGLM)
include(SetupSTB)
include(SetupAssimp)

set(SRC
	src/over/core/Camera.cpp
	src/over/core/Shader.cpp
	src/over/core/window/Context.cpp
	src/over/core/stb_impl.cpp
	src/over/core/Mesh.cpp
	src/over/core/Model.cpp
	src/over/core/Transform.cpp
	src/over/core/opengl/VAO.cpp
	src/over/core/opengl/VBO.cpp
	src/over/core/opengl/IBO.cpp
	src/over/core/opengl/Texture2D.cpp
	src/over/core/opengl/Framebuffer.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRC})
add_library(over::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE fmt::fmt
	PRIVATE glfw
	PRIVATE glad::glad
	PRIVATE glm::glm
	PRIVATE assimp::assimp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
