#include <assert.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "./include/glad/glad.h"
#include "./include/opengl_objects/opengl_objects.h"
#include "./include/shader_class/shader_class.h"
#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

int main() {
  if (glfwInit() != GLFW_TRUE) {
    std::cout << "GLFW Initialization Failed";

    glfwTerminate();
    return -1;
  }

  glfwWindowHint(GLFW_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT,
                                        "render-base", nullptr, nullptr);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;

    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to Initialize GLAD";

    glfwTerminate();
    return 1;
  }

  glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  engine::Shader shaderProgram("vertex_shader.vs", "fragment_shader.fs");

  gl_object::VAO vertex_array_object;
  vertex_array_object.bind_vao();

  gl_object::VBO vertex_buffer_object(vertices, sizeof(vertices));

  vertex_array_object.link_vbo(vertex_buffer_object, 0);

  vertex_array_object.unbind_vao();
  vertex_buffer_object.unbind_vbo();

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use_shader_program();

    vertex_array_object.bind_vao();
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  vertex_array_object.delete_vao();
  vertex_buffer_object.delete_vbo();

  shaderProgram.delete_shader_program();
  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;
}