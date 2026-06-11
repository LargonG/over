project(features)

include(SetupFMT)
include(SetupGLFW)
include(SetupGlad)
include(SetupGLM)
include(SetupAssimp)
include(SetupSTB)

include(utils/ConfigureShaders)
include(utils/ConfigureSources)

set(p_features_sources
	Main.cpp
)

set_source_directory(p_src SOURCE_DIR "src/over" SOURCES ${p_features_sources})

add_executable(${PROJECT_NAME} ${p_src})

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

set(shaders
	MainVertex.shader
	MainFragment.shader
)

compile_shaders(
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders"
	DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders"
	SOURCES ${shaders}
)


target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
