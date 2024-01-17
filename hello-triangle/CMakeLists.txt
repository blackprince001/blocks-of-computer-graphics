cmake_minimum_required( VERSION 3.5 )

project( Graphics-Hello-Triangle )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )

add_subdirectory( glfw )

option( GLFW-CMAKE-STARTER-USE-GLFW-GLAD "Use GLAD from GLFW" ON )

if( GLFW-CMAKE-STARTER-USE-GLFW-GLAD )
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
endif()

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

# set the main source for programs here.
set( MAIN_SRC
        main.cpp
        )

add_executable( Graphics-Hello-Triangle WIN32 ${MAIN_SRC} ${GLAD_GL} )
target_link_libraries( Graphics-Hello-Triangle ${OPENGL_LIBRARIES} glfw )

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'Graphics-Hello-Triangle' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Graphics-Hello-Triangle )
    endif()
endif()
