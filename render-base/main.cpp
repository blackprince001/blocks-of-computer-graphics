#include <assert.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "./lib/include/glad/glad.h"
#include "./lib/render.hpp"
#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

int main() {
    engine::Renderer r = engine::Renderer();

    r.set_window_size(SCREENWIDTH, SCREENHEIGHT);
    r.set_window_name("Testing Renderer Class.");

    r.create_window();
    r.create_opengl_context();

    r.initial_glad();
    // r.initial_viewport();

    // add vertex data for a triangle
    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};

    // initialize the id for an opengl object
    GLuint vertex_buffer_object{};
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    GLuint vertex_array_object{};
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    engine::Shader shader_program =
        engine::Shader("./vertex_shader.vs", "./fragment_shader.fs");

    r.render(shader_program, vertex_array_object);

    r.close_renderer();
}