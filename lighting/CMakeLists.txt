project(lighting CXX)

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
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/lightFragment.shader
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/lightFragment.shader COPYONLY
)

set(TEXTURES
    container2.png
    container2_specular.png
)

foreach(item IN LISTS TEXTURES)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures/${item}
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures)
endforeach()

add_executable(lighting main.cpp "mesh.hpp" "mesh.cpp")

target_include_directories(lighting PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(lighting PRIVATE fmt::fmt
                               PRIVATE glfw
                               PRIVATE glad::glad
                               PRIVATE glm::glm)
