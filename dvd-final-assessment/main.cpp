// Name: Appiah Boadu Prince Kwabena
// Index Number: 3024420

#include "lib/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
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

constexpr int WINDOW_WIDTH{800};
constexpr int WINDOW_HEIGHT{600};

const float vertices[] = {
    -0.125f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.125f,  -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.125f,  -0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.125f, -0.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

float curr_time, last_time, delta_time;
bool is_paused = false;

enum Move { UP, DOWN, LEFT, RIGHT };
Move direction = RIGHT;

const char *parse_move(Move move) {
  if (move == UP)
    return "UP";
  if (move == DOWN)
    return "DOWN";
  if (move == RIGHT)
    return "RIGHT";
  if (move == LEFT)
    return "LEFT";

  return "NONE";
}

void processInputs(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    is_paused = !is_paused;
}

unsigned int uniform_locator(GLuint shader_program, const GLchar *name) {
  int uniform_located = glGetUniformLocation(shader_program, name);
  if (uniform_located == -1)
    std::cout << "Could not find uniform in shader";

  return uniform_located;
}

// movement
void actions(GLFWwindow *window, GLuint shader_program, glm::mat4 &trans) {
  glUseProgram(shader_program);

  if (!is_paused) {
    if (direction == RIGHT) {
      trans = glm::translate(trans, glm::vec3(0.55f, 0.5f, 0.0f) * delta_time);

      if (trans[3][0] >= 0.85f) {
        direction = UP;
      }
    } else if (direction == UP) {
      trans = glm::translate(trans, glm::vec3(-0.55f, 0.5f, 0.0f) * delta_time);

      if (trans[3][1] >= 1.5f) {
        direction = LEFT;
      }
    } else if (direction == LEFT) {
      trans =
          glm::translate(trans, glm::vec3(-0.55f, -0.5f, 0.0f) * delta_time);

      if (trans[3][0] <= -0.85f) {
        direction = DOWN;
      }
    } else if (direction == DOWN) {
      trans = glm::translate(trans, glm::vec3(0.55f, -0.5f, 0.0f) * delta_time);

      if (trans[3][1] <= -0.02f) {
        direction = RIGHT;
      }
    }

    std::cout << "Logging info - pos: (" << trans[3][0] << ", " << trans[3][1]
              << "), dir: " << parse_move(direction) << std::endl;
  }
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

int main() {
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

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  auto vertex_shader_path = get_absolute_path("../vertex.vs");
  auto fragment_shader_path = get_absolute_path("../fragment.fs");

  auto read_vshader_source = read_from_file(vertex_shader_path);
  const char *vertex_shader_source = read_vshader_source.c_str();

  auto read_fshader_source = read_from_file(fragment_shader_path);
  auto fragment_shader_source = read_fshader_source.c_str();

  GLint success;
  char info_log[512];

  GLuint vertex_array_object, vertex_buffer_object;
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);

  glGenBuffers(1, &vertex_buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));

  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << info_log << std::endl;
  }

  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << info_log << std::endl;
  }

  auto shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << info_log << std::endl;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUseProgram(shader_program);

  auto transform_loc = uniform_locator(shader_program, "transform");
  glm::mat4 trans = glm::mat4(1.0f);

  while (!glfwWindowShouldClose(window)) {
    processInputs(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);

    curr_time = glfwGetTime();
    delta_time = curr_time - last_time;
    last_time = curr_time;

    actions(window, shader_program, trans);
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));

    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}
