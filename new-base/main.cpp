#include "lib/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

constexpr int WINDOW_WIDTH{800};
constexpr int WINDOW_HEIGHT{600};

constexpr auto vertex_shader_1_source{R"(
#version 330

layout (location = 0) in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0f);
}
)"};

constexpr auto vertex_shader_2_source{R"(
#version 330

layout (location = 0) in vec3 position;
uniform float offset_x;

void main() {
    gl_Position = vec4(position.x, -position.y + offset_x, position.z, 1.0f);
}
)"};

constexpr auto fragment_shader_1_source{R"(
#version 330

out vec4 fragment_color;

void main() {
    fragment_color = vec4(0.2f, 0.2f, 0.2f, 1.0f);
}
)"};

constexpr auto fragment_shader_2_source{R"(
#version 330

out vec4 fragment_color;

void main() {
    fragment_color = vec4(0.6f, 0.1f, 0.4f, 1.0f);
}
)"};

void log_shader_error(const unsigned int shader,
                      const std::string &message = "") {
  constexpr unsigned int LOG_BUFFER_SIZE_BYTES{512};
  char log_info[LOG_BUFFER_SIZE_BYTES];

  glGetShaderInfoLog(shader, LOG_BUFFER_SIZE_BYTES, nullptr, log_info);

  if (!message.empty()) {
    std::cout << message << std::endl;
  }
  std::cout << log_info << std::endl;

  std::exit(EXIT_FAILURE);
}

int main() {
  const float vertices[]{
      // First triangle
      -0.75f, 0.25f, 0.0f, -0.25f, 0.25f, 0.0f, -0.50f, 0.75f, 0.0f,

      // Second triangle
      0.25f, -0.75f, 0.0f, 0.75f, -0.75f, 0.0f, 0.50f, -0.25f, 0.0f};

  const float offset{-1.0f};

  if (!glfwInit()) {
    std::cout << "Failed to initialize glfw\n";
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window{glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Title",
                               nullptr, nullptr)};

  if (!window) {
    std::cout << "Failed to create window\n";
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  unsigned int vertex_array_object{}, vertex_buffer_object{};

  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);

  glGenBuffers(1, &vertex_buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        nullptr);
  glEnableVertexAttribArray(0);

  auto program_shader_1{glCreateProgram()};
  auto program_shader_2{glCreateProgram()};

  auto vertex_shader_1{glCreateShader(GL_VERTEX_SHADER)};
  auto vertex_shader_2{glCreateShader(GL_VERTEX_SHADER)};

  auto fragment_shader_1{glCreateShader(GL_FRAGMENT_SHADER)};
  auto fragment_shader_2{glCreateShader(GL_FRAGMENT_SHADER)};

  glShaderSource(vertex_shader_1, 1, &vertex_shader_1_source, nullptr);
  glShaderSource(vertex_shader_2, 1, &vertex_shader_2_source, nullptr);
  glShaderSource(fragment_shader_1, 1, &fragment_shader_1_source,
                 nullptr);
  glShaderSource(fragment_shader_2, 1, &fragment_shader_2_source,
                 nullptr);

  int successful{};

  glCompileShader(vertex_shader_1);
  glGetShaderiv(vertex_shader_1, GL_COMPILE_STATUS, &successful);

  if (!successful) {
    log_shader_error(vertex_shader_1,
                     "Error::Shader::Vertex::Compilation");
  }

  glCompileShader(vertex_shader_2);
  glGetShaderiv(vertex_shader_2, GL_COMPILE_STATUS, &successful);

  if (!successful) {
    log_shader_error(vertex_shader_2,
                     "Error::Shader::Vertex::Compilation");
  }

  glCompileShader(fragment_shader_1);
  glGetShaderiv(fragment_shader_1, GL_COMPILE_STATUS, &successful);

  if (!successful) {
    log_shader_error(fragment_shader_1,
                     "Error::Shader::Fragment::Compilation");
  }

  glCompileShader(fragment_shader_2);
  glGetShaderiv(fragment_shader_2, GL_COMPILE_STATUS, &successful);

  if (!successful) {
    log_shader_error(fragment_shader_2,
                     "Error::Shader::Fragment::Compilation");
  }

  glAttachShader(program_shader_1, vertex_shader_1);
  glAttachShader(program_shader_1, fragment_shader_1);
  glAttachShader(program_shader_2, vertex_shader_2);
  glAttachShader(program_shader_2, fragment_shader_2);

  glLinkProgram(program_shader_1);
  glGetShaderiv(program_shader_1, GL_LINK_STATUS, &successful);

  if (!successful) {
    log_shader_error(program_shader_1,
                     "Error::Shader::Program::Linking");
  }

  glLinkProgram(program_shader_2);
  glGetShaderiv(program_shader_2, GL_LINK_STATUS, &successful);

  if (!successful) {
    log_shader_error(program_shader_2,
                     "Error::Shader::Program::Linking");
  }

  glValidateProgram(program_shader_1);
  glValidateProgram(program_shader_2);

  auto uniform_location{
      glGetUniformLocation(program_shader_2, "offset_x")};

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);

    glUseProgram(program_shader_1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(program_shader_2);
    glUniform1f(uniform_location, offset);
    glDrawArrays(GL_TRIANGLES, 3, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
