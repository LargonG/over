project(owlet_simple_app_example CXX)

add_executable(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

set(p_sources
    main.cpp
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
