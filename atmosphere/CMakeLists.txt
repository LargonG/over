project(atmosphere CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(utils/ConfigureSources)
include(utils/ConfigureShaders)
include(utils/ConfigureResources)

set(p_at_sources
)

set(p_at_modules
	Main.cppm
	app/App.cppm
	shapes/Sphere.cppm
)

set(p_at_headers

)

set(p_at_shaders
	Planet.vert
	Planet.frag
)

set_source_directory(p_src SOURCE_DIR "src/over/atmosphere/" SOURCES ${p_at_sources})
set_source_directory(p_modules SOURCE_DIR "src/over/atmosphere" SOURCES ${p_at_modules})
set_source_directory(p_headers SOURCE_DIR "include/over/atmosphere" SOURCES ${p_at_headers})

compile_shaders(
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders"
	DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders"
	SOURCES ${p_at_shaders}
)

add_executable(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_sources(${PROJECT_NAME}
	PRIVATE ${p_src}
	PRIVATE FILE_SET modules TYPE CXX_MODULES FILES ${p_modules}
	PRIVATE FILE_SET headers TYPE HEADERS FILES ${p_headers}
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE over::engine
	PRIVATE imgui::imgui
)
