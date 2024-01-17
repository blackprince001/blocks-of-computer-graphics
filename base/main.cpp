#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdlib>

// int main(void) {
//     GLFWwindow *window;

//     /* Initialize the library */
//     if (!glfwInit())
//         return -1;

//     /* Create a windowed mode window and its OpenGL context */
//     window = glfwCreateWindow(640, 480, "GLFW CMake starter", NULL, NULL);
//     if (!window) {
//         glfwTerminate();
//         return -1;
//     }

//     /* Make the window's context current */
//     glfwMakeContextCurrent(window);
//     glClearColor(0.4f, 0.3f, 0.4f, 0.0f);

//     /* Loop until the user closes the window */
//     while (!glfwWindowShouldClose(window)) {
//         /* Render here */
//         glClear(GL_COLOR_BUFFER_BIT);

//         /* Swap front and back buffers */
//         glfwSwapBuffers(window);

//         /* Poll for and process events */
//         glfwPollEvents();
//     }

//     glfwTerminate();
//     return 0;
// }

int main() {
    glfwInit();

    // Config for GLFW and OpenGL Legacy
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}