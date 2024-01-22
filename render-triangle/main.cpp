#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>

/* first callback to close the window when ESC is pressed. */
void close_window_on_esc(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Initialization failed.";
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Test Window", NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  // add vertex data for a triangle
  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                      0.0f,  0.0f,  0.5f, 0.0f};

  // initialize the id for an opengl object
  unsigned int vbo;
  glGenBUffers(1, &vbo);

  glBindTexture(GL_ARRAY_BUFFER, vbo);

  while (!glfwWindowShouldClose(window)) {
    close_window_on_esc(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}