cmake_minimum_required(VERSION 3.24)
project(dvd-final-assessment)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)
find_package(glfw3 3.3 REQUIRED)

add_executable(dvd-final-assessment main.cpp lib/glad.c)

target_include_directories(dvd-final-assessment SYSTEM PRIVATE lib/include)

target_link_libraries(dvd-final-assessment glm::glm)
target_link_libraries(dvd-final-assessment glfw)
