project(fractal CXX)

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex.shader
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/vertex.shader COPYONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment.shader
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment.shader COPYONLY
)

add_executable(fractal main.cpp)

target_include_directories(fractal PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(fractal PRIVATE fmt::fmt
                               PRIVATE glfw
                               PRIVATE glad::glad
                               PRIVATE glm::glm)