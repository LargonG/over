project(owlet CXX)

add_subdirectory(libs/glad2)


set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC)
add_library(over::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
)

if(MSVC)
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE /W4 /WX
    )
else()
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE -Wall -Wextra -Wpedantic -Werror
    )
endif()


set(p_core
    os/desktop.cpp
    os/monitor.cpp
    os/window.cpp
    
    gl/context.cpp
    gl/buffer.cpp
    gl/texture.cpp
    gl/frame_buffer.cpp
    gl/vertex_array.cpp
    gl/state.cpp
    gl/layout.cpp
    gl/shader.cpp
    gl/program.cpp

    gl/targets/buffer_target.cpp
    gl/targets/uniform_buffer.cpp
    
    debug/gl.cpp
    debug/log.cpp
)
list(TRANSFORM p_core PREPEND "src/owlet/")

set(p_hints
    cpp.hint
)
list(TRANSFORM p_hints PREPEND "src/")


file(GLOB_RECURSE p_headers "include/*.h")

target_sources(
    ${PROJECT_NAME}
    PRIVATE ${p_core}
    PRIVATE ${p_hints}
    PRIVATE FILE_SET headers TYPE HEADERS FILES ${p_headers}
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE fmt::fmt
    PUBLIC glad::glad2
    PUBLIC glfw
    PUBLIC glm::glm
    # PRIVATE assimp::assimp
)


add_subdirectory(examples/simple)
