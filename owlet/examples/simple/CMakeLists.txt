project(owlet_simple_example CXX)

add_executable(${PROJECT_NAME})

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

set(p_example
    main.cpp
)
list(TRANSFORM p_example PREPEND "src/")


target_sources(
    ${PROJECT_NAME}
    PRIVATE ${p_example}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE over::owlet
    PRIVATE fmt::fmt
)
