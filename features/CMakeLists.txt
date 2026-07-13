project(features)

include(utils/ConfigureShaders)
include(utils/ConfigureSources)
include(utils/ConfigureResources)

set(p_features_sources
	Main.cpp
)

set_source_directory(p_src SOURCE_DIR "src/over" SOURCES ${p_features_sources})

add_executable(${PROJECT_NAME} ${p_src})

target_link_libraries(${PROJECT_NAME}
	PRIVATE over::engine
	PRIVATE imgui::imgui
)

set(shaders
	ScreenVertex.shader
	ScreenFragment.shader

	DimensionVertex.shader
	DimensionGeometry.shader
	DimensionFragment.shader

	NormalDebugVertex.shader
	NormalDebugGeometry.shader
	NormalDebugFragment.shader

	SkyboxVertex.shader
	SkyboxFragment.shader
)

compile_shaders(
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders"
	DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders"
	SOURCES ${shaders}
)


set(m_build_mode DEBUG)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(m_build_mode RELEASE)
endif()

set_resources(${m_build_mode} DIRECTORY "resources")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
