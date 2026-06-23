project(basics CXX)

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

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE over::core
)
