#include "lib/include/glad/glad.h"
#include "lib/stb_image.hpp"

#include <GLFW/glfw3.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

constexpr int WINDOW_WIDTH{800};
constexpr int WINDOW_HEIGHT{600};

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

// This only works for POSIX paths. If you want it to work on Windows
// consider modifying this function with changes to the backslash
std::string get_absolute_path(const std::string &relative_path) {
  return std::filesystem::current_path().string() + "/" + relative_path;
}

std::string read_from_file(const std::string &filepath) {
  std::string file_contents;
  std::ifstream file;
  std::stringstream ss;

  file.open(filepath);
  if (!file.is_open()) {
    std::cout << "failed to load file: " << filepath << "\n";
  }

  ss << file.rdbuf();
  file_contents = ss.str();

  file.close();
  return file_contents;
}

void close_window_on_esc(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  const GLfloat gl_data[] = {0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                             0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                             -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

  const unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  auto vertex_shader_path = get_absolute_path("../vertex.vs");
  auto fragment_shader_path = get_absolute_path("../fragment.fs");

  auto read_vshader_source = read_from_file(vertex_shader_path);
  const char *vertex_shader_source = read_vshader_source.c_str();

  auto read_fshader_source = read_from_file(fragment_shader_path);
  auto fragment_shader_source = read_fshader_source.c_str();

  if (!glfwInit()) {
    std::cout << "Failed to initialize glfw\n";
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window{glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                               "DVD-animation Window", nullptr, nullptr)};

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

  unsigned int vertex_array_object{}, vertex_buffer_object{},
      element_buffer_object{};

  glGenVertexArrays(1, &vertex_array_object);

  glGenBuffers(1, &vertex_buffer_object);
  glGenBuffers(1, &element_buffer_object);

  glBindVertexArray(vertex_array_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gl_data), gl_data, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // rectangle for dvd image attribute and stride
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  // color attribute and stride
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coordinates attribute and stride
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(true);

  auto texture_image_path = get_absolute_path("../texture/dvd-logo.png");
  unsigned char *tex_data =
      stbi_load(texture_image_path.c_str(), &width, &height, &nr_channels, 0);

  if (tex_data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(tex_data);

  auto program_shader{glCreateProgram()};

  auto vertex_shader{glCreateShader(GL_VERTEX_SHADER)};

  auto fragment_shader{glCreateShader(GL_FRAGMENT_SHADER)};

  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

  int successful{};

  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &successful);

  if (!successful) {
    log_shader_error(vertex_shader, "Error::Shader::Vertex::Compilation");
  }

  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &successful);

  if (!successful) {
    log_shader_error(fragment_shader, "Error::Shader::Fragment::Compilation");
  }

  glAttachShader(program_shader, vertex_shader);
  glAttachShader(program_shader, fragment_shader);

  glLinkProgram(program_shader);
  glGetShaderiv(program_shader, GL_LINK_STATUS, &successful);

  if (!successful) {
    log_shader_error(program_shader, "Error::Shader::Program::Linking");
  }

  glValidateProgram(program_shader);

  while (!glfwWindowShouldClose(window)) {
    close_window_on_esc(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);

    glUseProgram(program_shader);
    glBindVertexArray(vertex_array_object);

    glBindTexture(GL_TEXTURE, texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glfwTerminate();
}
