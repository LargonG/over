project(basics CXX)

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex.shader
  ${CMAKE_CURRENT_BINARY_DIR}/shaders/vertex.shader COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment.shader
  ${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment.shader COPYONLY
)

set(TEXTURES
  awesomeface.png
  container.jpg
  wall.jpg
)

foreach(item IN LISTS TEXTURES)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures/${item}
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures)
endforeach()

add_executable(basics main.cpp)

target_include_directories(basics PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(basics PRIVATE fmt::fmt)
target_link_libraries(basics PRIVATE glfw)
target_link_libraries(basics PRIVATE glad::glad)
target_link_libraries(basics PRIVATE glm::glm)