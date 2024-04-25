// Name: Appiah Boadu Prince Kwabena
// Index Number: 3024420

#include "lib/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "lib/stb_image.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

constexpr int WINDOW_WIDTH { 800 };
constexpr int WINDOW_HEIGHT { 600 };

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

void processInputs(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

unsigned int uniform_locator(GLuint shader_program, const GLchar* name)
{
  int uniform_located = glGetUniformLocation(shader_program, name);
  if (uniform_located == -1)
    std::cout << "Could not find uniform in shader";

  return uniform_located;
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

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

  auto vertex_shader_path = get_absolute_path("../vertex.vs");
  auto fragment_shader_path = get_absolute_path("../fragment.fs");

  auto read_vshader_source = read_from_file(vertex_shader_path);
  const char* vertex_shader_source = read_vshader_source.c_str();

  auto read_fshader_source = read_from_file(fragment_shader_path);
  auto fragment_shader_source = read_fshader_source.c_str();

  const GLfloat gl_data[] = { 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f };

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

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(true);

  auto texture_image_path = get_absolute_path("../texture/dvd-logo.png");
  unsigned char* tex_data
      = stbi_load(texture_image_path.c_str(), &width, &height, &nr_channels, 0);

  if (tex_data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(tex_data);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  auto program_shader { glCreateProgram() };

  auto vertex_shader { glCreateShader(GL_VERTEX_SHADER) };

  auto fragment_shader { glCreateShader(GL_FRAGMENT_SHADER) };

  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

  int compile_flag {};

  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_flag);

  if (!compile_flag) {
    log_shader_error(vertex_shader, "Error::Shader::Vertex::Compilation");
  }

  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_flag);

  if (!compile_flag) {
    log_shader_error(fragment_shader, "Error::Shader::Fragment::Compilation");
  }

  glAttachShader(program_shader, vertex_shader);
  glAttachShader(program_shader, fragment_shader);

  glLinkProgram(program_shader);
  glGetShaderiv(program_shader, GL_LINK_STATUS, &compile_flag);

  if (!compile_flag) {
    log_shader_error(program_shader, "Error::Shader::Program::Linking");
  }

  glValidateProgram(program_shader);
  glUseProgram(program_shader);
  glUniform1i(uniform_locator(program_shader, "texture1"), 0);

  // float current_frame {}, last_frame {}, delta_frame {};

  // glm::vec2 dvd_texture_position(0.0f, 0.0f);
  // glm::vec2 dvd_texture_velocity(0.001f, 0.001f);

  // auto transform_loc = uniform_locator(program_shader, "transform");
  // glm::mat4 trans = glm::mat4(1.0f);

  // float dvd_texture_halfwidth = 0.1f;
  // float dvd_texture_halfheight = 0.1f;

  // const float WINDOW_LEFT = -1.0f;
  // const float WINDOW_RIGHT = 1.0f;
  // const float WINDOW_BOTTOM = -1.0f;
  // const float WINDOW_TOP = 1.0f;

  while (!glfwWindowShouldClose(window)) {
    processInputs(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);

    // current_frame = glfwGetTime();
    // delta_frame = current_frame - last_frame;
    // last_frame = current_frame;

    // if (dvd_texture_position.x - dvd_texture_halfwidth < WINDOW_LEFT) {
    //   dvd_texture_velocity.x = -dvd_texture_velocity.x;

    // } else if (dvd_texture_position.x + dvd_texture_halfwidth > WINDOW_RIGHT)
    // {
    //   dvd_texture_velocity.x = -dvd_texture_velocity.x;
    // }
    // if (dvd_texture_position.y - dvd_texture_halfheight < WINDOW_BOTTOM) {
    //   dvd_texture_velocity.y = -dvd_texture_velocity.y;

    // } else if (dvd_texture_position.y + dvd_texture_halfheight > WINDOW_TOP)
    // {
    //   dvd_texture_velocity.y = -dvd_texture_velocity.y;
    // }

    // dvd_texture_position += dvd_texture_velocity * delta_frame;

    // std::cout << "x: " << dvd_texture_position.x
    //           << " y: " << dvd_texture_position.y << std::endl;

    // trans = glm::translate(
    //     trans, glm::vec3(dvd_texture_position.x, dvd_texture_position.y,
    //     0.0f));

    glUseProgram(program_shader);
    // glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));
    glBindVertexArray(vertex_array_object);

    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);
  glDeleteVertexArrays(1, &vertex_array_object);

  glfwTerminate();
}
