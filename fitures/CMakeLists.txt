project(fitures)

include(SetupFMT)
include(SetupGLFW)
include(SetupGlad)
include(SetupGLM)
include(SetupAssimp)
include(SetupSTB)

set(SRC 
	src/over/Main.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
	PRIVATE fmt::fmt
	PRIVATE glfw
	PRIVATE glad::glad
	PRIVATE glm::glm
	PRIVATE assimp::assimp
	PRIVATE over::core
	PRIVATE over::engine
	PRIVATE imgui::imgui
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
