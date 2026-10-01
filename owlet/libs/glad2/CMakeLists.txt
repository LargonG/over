project(glad2 LANGUAGES C)

add_library(${PROJECT_NAME} EXCLUDE_FROM_ALL)
add_library(glad::glad2 ALIAS ${PROJECT_NAME})

set(p_headers
    glad/gl.h
    KHR/khrplatform.h
)
list(TRANSFORM p_headers PREPEND "include/")

set(p_sources
    gl.c
)
list(TRANSFORM p_sources PREPEND "src/")

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET headers TYPE HEADERS BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" FILES ${p_headers}
    PRIVATE ${p_sources}
)
