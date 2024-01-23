cmake_minimum_required(VERSION 3.24)
project(learnopengl)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.3 REQUIRED)

add_executable(learnopengl main.cpp lib/glad.c)

target_include_directories(learnopengl SYSTEM PRIVATE lib/include)

target_link_libraries(learnopengl glfw)
