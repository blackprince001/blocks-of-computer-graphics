#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ratio>
#include <vector>

#include "./lib/include/glad/glad.h"
#include "./lib/render.h"
#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

void close_window_on_esc(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    Renderer renderer{};

    renderer.set_window_size(SCREENWIDTH, SCREENHEIGHT);
    renderer.set_window_name("Testing Renderer Class.");

    auto window = renderer.create_window();

    if (window == NULL) {
        renderer.close_renderer();
    }

    renderer.create_opengl_context(window);

    renderer.initial_glad();
    renderer.initial_viewport(window);

    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};

    unsigned long size_of_vertices = sizeof(vertices);

    GLuint vertex_buffer_object{};
    GLuint vertex_array_object{};

    auto vertex_object = VertexObject(
        vertices, size_of_vertices, vertex_buffer_object, vertex_array_object);

    const char *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    ShaderObject vertex_shader =
        ShaderObject(GL_VERTEX_SHADER, vertex_shader_source);

    vertex_shader.compile();

    const char *fragment_shader_source =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    ShaderObject frag_shader =
        ShaderObject(GL_FRAGMENT_SHADER, fragment_shader_source);

    frag_shader.compile();

    std::vector<ShaderObject> shaders = {vertex_shader, frag_shader};
    ShaderProgramObject shader_program = ShaderProgramObject(shaders);

    shader_program.compile_shader_program();

    while (!glfwWindowShouldClose(window)) {
        close_window_on_esc(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.render();
        vertex_object.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    renderer.close_renderer();
}
