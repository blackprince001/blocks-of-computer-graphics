#include "lib/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
  // Initialize GLFW
  if (!glfwInit()) {
    std::cout << "Failed to initialize glfw\n";
    std::exit(EXIT_FAILURE);
  }

  // Set GLFW Config (version and core profile)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window
  auto window { glfwCreateWindow(800, 600, "Title", nullptr, nullptr) };

  if (!window) {
    std::cout << "Failed to create window\n";
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  // Make a context for the window
  glfwMakeContextCurrent(window);

  // Initialize Glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  // Create a mainloop for your window and color background
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // terminate after you are done and that's how you create a window with opengl
  glfwTerminate();
}
