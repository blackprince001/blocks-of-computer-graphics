#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include "lib/stb_image.hpp"

const int WINDOW_WIDTH { 800 };
const int WINDOW_HEIGHT { 600 };

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void close_on_esc_callback(GLFWwindow* window);
void log_shader_error(const unsigned int shader, const std::string& message);
void check_shader_compilations(
    GLuint& vertex_shader, GLuint& fragment_shader, GLuint& program_shader);
// std::string read_from_file(const std::string& filepath);
// std::string get_absolute_path(const std::string& relative_path);

int main()
{
  if (!glfwInit()) {
    std::cout << "Failed to initialize glfw\n";
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window { glfwCreateWindow(
      WINDOW_WIDTH, WINDOW_HEIGHT, "DVD-animation Window", nullptr, nullptr) };

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

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // auto vertex_shader_path = get_absolute_path("../shaders/vertex.vert");
  // auto fragment_shader_path = get_absolute_path("../shaders/fragment.frag");

  // auto read_vshader_source = read_from_file(vertex_shader_path);
  auto vertex_shader_source = R"(
    #version 330 core

    layout(location = 0) in vec3 coords;

    void main() {
        gl_Position = vec4(coords, 1.0);
    }
)";

  // auto read_fshader_source = read_from_file(fragment_shader_path);
  auto fragment_shader_source = R"(
    #version 330 core

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

  // auto texture_image_path = get_absolute_path("../texture/dvd-logo.png");

  const GLfloat gl_data[] = { 0.1f, 0.1f, 0.0f, 1.0f, 1.0f, 0.1f, -0.1f, 0.0f,
    1.0f, 0.0f, -0.1f, -0.1f, 0.0f, 0.0f, 0.0f, -0.1f, 0.1f, 0.0f, 0.0f, 1.0f };

  const unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

  GLuint vertex_array_object, vertex_buffer_object, element_buffer_object;

  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);

  glGenBuffers(1, &vertex_buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gl_data), gl_data, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
      1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

  GLuint program_shader { glCreateProgram() };

  GLuint vertex_shader { glCreateShader(GL_VERTEX_SHADER) };

  GLuint fragment_shader { glCreateShader(GL_FRAGMENT_SHADER) };

  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

  check_shader_compilations(vertex_shader, fragment_shader, program_shader);

  while (!glfwWindowShouldClose(window)) {
    close_on_esc_callback(window);

    glUseProgram(program_shader);
    glBindVertexArray(vertex_array_object);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void close_on_esc_callback(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void log_shader_error(
    const unsigned int shader, const std::string& message = "")
{
  const unsigned int LOG_BUFFER_SIZE_BYTES = 512;
  char log_info[LOG_BUFFER_SIZE_BYTES];

  glGetShaderInfoLog(shader, LOG_BUFFER_SIZE_BYTES, nullptr, log_info);

  if (!message.empty()) {
    std::cout << message << std::endl;
  }
  std::cout << log_info << std::endl;

  std::exit(EXIT_FAILURE);
}

// std::string get_absolute_path(const std::string& relative_path)
// {
//   return std::filesystem::current_path().string() + "/" + relative_path;
// }

// std::string read_from_file(const std::string& filepath)
// {
//   std::string file_contents;
//   std::ifstream file;
//   std::stringstream ss;

//   file.open(filepath);
//   if (!file.is_open()) {
//     std::cout << "failed to load file: " << filepath << "\n";
//   }

//   ss << file.rdbuf();
//   file_contents = ss.str();
//   file.close();

//   return file_contents;
// }

void check_shader_compilations(
    GLuint& vertex_shader, GLuint& fragment_shader, GLuint& program_shader)
{
  int compile_flag {};

  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_flag);

  if (!compile_flag) {
    log_shader_error(vertex_shader, "Error::Shader::Vertex::Compilation");
  }

  std::cout << "Successfully compiled vertex shader" << std::endl;

  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_flag);

  if (!compile_flag) {
    log_shader_error(fragment_shader, "Error::Shader::Fragment::Compilation");
  }

  std::cout << "Successfully compiled fragment shader" << std::endl;

  glAttachShader(program_shader, vertex_shader);
  glAttachShader(program_shader, fragment_shader);

  glLinkProgram(program_shader);
  glGetShaderiv(program_shader, GL_LINK_STATUS, &compile_flag);

  if (!compile_flag) {
    log_shader_error(program_shader, "Error::Shader::Program::Linking");
  }

  std::cout
      << "Successfully linked vertex and fragment shaders into shader program"
      << std::endl;
}