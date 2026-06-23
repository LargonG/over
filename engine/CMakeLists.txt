project(engine CXX)

include(utils/ConfigureSources)

set(p_engine_sources
	App.cpp
	Input.cpp
)

set_source_directory(p_src SOURCE_DIR "src/over/engine" SOURCES ${p_engine_sources})

add_library(${PROJECT_NAME} STATIC ${p_src})
add_library(over::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC over::core
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
