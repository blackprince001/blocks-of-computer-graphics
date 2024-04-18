cmake_minimum_required(VERSION 3.24)
project(dvd-final-assessment)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.3 REQUIRED)

add_executable(dvd-final-assessment main.cpp lib/glad.c)

target_include_directories(dvd-final-assessment SYSTEM PRIVATE lib/include)

target_link_libraries(dvd-final-assessment glfw)
