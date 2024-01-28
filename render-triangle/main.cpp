#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ratio>

#include "./lib/glad.c"
#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"

const int ScrenWidth = 800;
const int ScreenHeight = 600;

void InitializeGlfw()
{
  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Initialization failed.";
  }
}

void InitializeViewport(GLFWwindow* window, int width, int height)
{
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
}

void setGlfwConfig()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Cleanup()
{
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

/* first callback to close the window when ESC is pressed. */
void close_window_on_esc(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main()
{
  InitializeGlfw();

  setGlfwConfig();

  GLFWwindow* window
      = glfwCreateWindow(ScrenWidth, ScreenHeight, "Test Window", NULL, NULL);

  // InitializeViewport(window, ScrenWidth, ScreenHeight);

  if (window == NULL) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  // add vertex data for a triangle
  float vertices[]
      = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f };

  // initialize the id for an opengl object
  unsigned int vertex_buffer_object {};
  glGenBuffers(1, &vertex_buffer_object);

  // we bind the our buffer id to it's type
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

  // so that we can afford to call that bind to copy all the buffers
  // with glBufferData which has all the vertice data into memory.

  // the last argument of glBufferData determines how we manage the data
  // on the GPU
  // GL_STREAM_DRAW: the data is set only once and used by the GPU at
  // most a few times.
  // GL_STATIC_DRAW: the data is set only once and used many times.
  // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  while (!glfwWindowShouldClose(window)) {
    close_window_on_esc(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  Cleanup();
}