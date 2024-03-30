project('render-base', ['cpp', 'c'],
  version : '0.1',
  default_options : ['warning_level=1', 'cpp_std=c++20'])

glfw_proj = subproject('glfw')
glfw_dep = glfw_proj.get_variable('glfw_dep')

inc_dir = include_directories('include')

inc_glad = include_directories('include/glad')
 
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

# Own opengl_object library for creating vbos, vaos, and ebos
inc_gl_object = include_directories('include/opengl_objects')
 
lib_gl_object_files = files('src/opengl_objects/opengl_objects.cpp')
 
lib_gl_object = static_library(
   'gl_object',
   lib_gl_object_files,
   include_directories: inc_gl_object
)

# custom shader_class for compiling shaders.
inc_engine = include_directories('include/shader_class')
 
lib_engine_files = files('src/shader_class/shader_class.cpp')
 
lib_engine = static_library(
   'engine',
   lib_engine_files,
   include_directories: inc_engine
)


executable('render-base',
           'main.cpp', include_directories: inc_dir,
           dependencies: [glfw_dep, idep_glad], 
           link_with: [lib_gl_object, lib_engine], link_args: '-lGL')