cmake_minimum_required(VERSION 3.10)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)

include_directories(external/glad/include)

add_executable(game src/main.cpp external/glad/src/glad.c)

target_link_libraries(game
    glfw
    dl
)
