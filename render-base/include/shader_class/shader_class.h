#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "../glad/glad.h"
#include <cerrno>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string get_file_contents(const char *filename);
enum ShaderType { VERTEX, FRAGMENT, PROGRAM };
std::string get_string_from_enum(ShaderType e);

namespace engine {

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexFile, const char *fragmentFile);

  void check_compile_errors(GLuint shader, std::string type);
  void use_shader_program();
  void delete_shader_program();
};

} // namespace engine
#endif