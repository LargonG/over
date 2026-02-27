project(morphing CXX)

include(SetupFMT)
include(SetupGLFW)
include(SetupGlad)
include(SetupGLM)
include(SetupAssimp)
include(SetupSTB)
include(SetupImGui)

set(SHADERS
	vertex.shader
	fragment.shader
)

foreach(SHADER IN LISTS SHADERS)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER}
		${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER} COPYONLY
	)
endforeach()

set(MODELS
	cube	
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
	src/over/Main.cpp
	src/over/examples/morphing/DirectionalLight.cpp
	src/over/examples/morphing/SpotLight.cpp
	src/over/examples/morphing/Light.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
	PRIVATE fmt::fmt
	PRIVATE glfw
	PRIVATE glad::glad
	PRIVATE glm::glm
	PRIVATE assimp::assimp
	PRIVATE over::core
	PRIVATE imgui::imgui
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
