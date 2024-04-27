#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

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
std::string read_from_file(const std::string& filepath);
std::string get_absolute_path(const std::string& relative_path);

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

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);

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

std::string get_absolute_path(const std::string& relative_path)
{
  return std::filesystem::current_path().string() + "/" + relative_path;
}

std::string read_from_file(const std::string& filepath)
{
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