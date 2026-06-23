project(fractal CXX)

include(SetupImGui)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex.shader
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/vertex.shader COPYONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment.shader
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment.shader COPYONLY
)

add_executable(fractal main.cpp)

target_link_libraries(fractal
    PRIVATE imgui::imgui
    PRIVATE over::core
)
