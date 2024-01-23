project('render-triangle', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

glfw_proj = subproject('glfw')
glfw_dep = glfw_proj.get_variable('glfw_dep')

# glew_proj = subproject('glew')
# glew_dep = glew_proj.get_variable('glew_dep')

incdirs = include_directories('lib')

# using the Cmake Copy and using targets as dependencies
# cmake = import('cmake')

# glfw_proj = cmake.subproject('glfw')
# glfw_dep = glfw_proj.dependency('all')

executable('render-triangle',
           'main.cpp', dependencies: [glfw_dep], link_args: '-lGL', include_directories: incdirs)
