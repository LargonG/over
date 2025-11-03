project(core CXX)

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)


add_library(core STATIC "camera.cpp" "shader.cpp")

target_include_directories(core PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(core PRIVATE fmt::fmt)
target_link_libraries(core PRIVATE glfw)
target_link_libraries(core PRIVATE glad::glad)
target_link_libraries(core PRIVATE glm::glm)