project('render-square', ['cpp', 'c'],
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

# add glfw dependency
glfw_proj = subproject('glfw')
glfw_dep = glfw_proj.get_variable('glfw_dep')

# add glm dependency
glm_proj = subproject('glm', default_options : ['default_library=static'])
glm_dep = glm_proj.get_variable('glm_dep')

# add glad dependency
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

# make executable with the dependencies
executable('render-square',
           'main.cpp', dependencies: [glfw_dep, idep_glad, glm_dep], link_args: '-lGL')