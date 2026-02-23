project(fractal CXX)

include(SetupFMT)
include(SetupGlad)
include(SetupGLFW)
include(SetupGLM)
include(SetupSTB)
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
    PRIVATE fmt::fmt
    PRIVATE glfw
    PRIVATE glad::glad
    PRIVATE glm::glm
    PRIVATE imgui::imgui
    PRIVATE over::core
)

target_include_directories(fractal PRIVATE ${Stb_INCLUDE_DIR})
