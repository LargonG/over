project(models CXX)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex.shader
	${CMAKE_CURRENT_BINARY_DIR}/shaders/vertex.shader COPYONLY
)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment.shader
	${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment.shader COPYONLY
)

set(MODELS
	backpack	
)

foreach(modir IN LISTS MODELS)
  file(GLOB MODEL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/${modir}/*)
  message(STATUS "model is ${MODEL}")
  foreach(item IN LISTS MODEL)
    message("item is ${item}")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${item}
	  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/${modir})
  endforeach()
endforeach()

add_executable(models "src/main.cpp" "src/mmesh.cpp" "src/mmodel.cpp")

target_include_directories(models PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(models
	PRIVATE fmt::fmt
	PRIVATE glfw
	PRIVATE glad::glad
	PRIVATE glm::glm
	PRIVATE assimp::assimp
)
