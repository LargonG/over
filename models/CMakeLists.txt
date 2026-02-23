project(models CXX)

include(SetupFMT)
include(SetupGlad)
include(SetupGLFW)
include(SetupGLM)
include(SetupSTB)
include(SetupImGui)
include(SetupAssimp)

set(SHADERS
	vertex.shader
	fragment.shader
	stencil_vertex.shader
	stencil_fragment.shader
)

foreach(SHADER IN LISTS SHADERS)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER}
		${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER} COPYONLY
	)
endforeach()

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

set(SRC
	src/over/main.cpp
	src/over/examples/models/mmesh.cpp
	src/over/examples/models/mmodel.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
	PRIVATE fmt::fmt
	PRIVATE glfw
	PRIVATE glad::glad
	PRIVATE glm::glm
	PRIVATE assimp::assimp
	PRIVATE over::core
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
