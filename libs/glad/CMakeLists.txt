cmake_minimum_required(VERSION 3.10)

project(glad LANGUAGES C)

add_library(${PROJECT_NAME} src/glad.c)
add_library(glad::glad ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)