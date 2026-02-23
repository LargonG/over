project(lighting CXX)

include(SetupFMT)
include(SetupGlad)
include(SetupGLFW)
include(SetupGLM)
include(SetupSTB)

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
    lighting_maps_specular_color.png
    matrix.jpg
)

foreach(item IN LISTS TEXTURES)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures/${item}
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures)
endforeach()

set(SRC
    main.cpp
    mesh.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
    PRIVATE fmt::fmt
    PRIVATE glfw
    PRIVATE glad::glad
    PRIVATE glm::glm
    PRIVATE over::core
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
