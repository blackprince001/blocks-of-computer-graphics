#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>

/* first callback to close the window when ESC is pressed. */
void close_window_on_esc(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

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
        close_window_on_esc(window);

        // for some reason the render here does not work and I dont know
        // why yet
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* we look out for events and swapbuffers for our window.The
         * glfwPollEvents function checks if any events are triggered
         * (like keyboard input or mouse movement events), updates the
         * state, and calls the corresponding functions (which we can
         * register via callback methods).
         */
        glfwPollEvents();
        /* The glfwSwapBuffers will swap the color buffer (a large 2D
         * buffer that contains color values for each pixel in GLFW’s
         * window) that is used to render to during this render
         * iteration and show it as output to the screen.*/
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}