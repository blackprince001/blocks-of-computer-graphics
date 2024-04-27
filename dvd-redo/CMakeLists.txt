cmake_minimum_required(VERSION 3.24)
project(dvd-redo)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)
add_executable(dvd-redo main.cpp)

find_package(glfw3 3.3 REQUIRED)

add_subdirectory(glad/)
target_link_libraries(dvd-redo PRIVATE glad)

target_link_libraries(dvd-redo PRIVATE glfw glm::glm)
# target_link_libraries(dvd-redo glfw)
