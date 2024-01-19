#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>

int main() {
    /* glfwInit returns GLFW_TRUE if the initialization process succeeds
     * otherwise GFLW_FALSE*/

    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Initialization failed.";
    }

    /* Config for GLFW and OpenGL Legacy */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* We create a GLFW window and assign the attributes. */
    GLFWwindow *window =
        glfwCreateWindow(800, 600, "Test Window", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /* Since an OpenGL context can have as many windows or drawable
     * items on a screen, we can create as many windows we like and
     * place them in the OpenGL Context.*/
    glfwMakeContextCurrent(window);

    /* The context will remain current until you make another context
    current or until the window owning the current context is
    destroyed.*/

    /* We create some eventListener to monitor when we close a window in
     any
     * way and we accomplish that with `glfwWindownShouldClose()` which
     returns
     * a flag if our window in a particular context has been closed.
     * We can also use callbacks to achieve the same effect by creating
     * a function callback and use glfwSetCallback(window, callback) to
     handle an
     * event of a particular key pressed or action to close the window.
    */
    while (!glfwWindowShouldClose(window)) {
        // Keep running
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}