project(owlet_simple_3d_example CXX)

add_executable(${PROJECT_NAME})

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED TRUE
)

set(p_sources
    main.cpp
    example_app.cpp
    example_scene.cpp
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE ${p_sources}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE over::owlet
    PRIVATE fmt::fmt
)
