project('render_triangle_multicolor', ['cpp', 'c'],
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

glfw_proj = subproject('glfw')
glfw_dep = glfw_proj.get_variable('glfw_dep')

# glew_proj = subproject('glew')
# glew_dep = glew_proj.get_variable('glew_dep')

# incdirs = include_directories('lib/')

inc_glad = include_directories('lib/include/glad')
 
libglad_files = files('lib/glad.c')
 
libglad = static_library(
   'glad',
   libglad_files,
   include_directories: inc_glad
)
 
idep_glad = declare_dependency(
   link_with: libglad,
   include_directories: inc_glad,
)


# using the Cmake Copy and using targets as dependencies
# cmake = import('cmake')

# glfw_proj = cmake.subproject('glfw')
# glfw_dep = glfw_proj.dependency('all')

executable('render_triangle_multicolor',
           'main.cpp', dependencies: [glfw_dep, idep_glad], link_args: '-lGL')