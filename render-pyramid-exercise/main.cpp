#include <assert.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "./lib/include/glad/glad.h"
#include "./subprojects//glm-1.0.1/glm/gtc/matrix_transform.hpp"
#include "./subprojects//glm-1.0.1/glm/gtc/type_ptr.hpp"
#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"
#include "./subprojects/glm-1.0.1/glm/glm.hpp"

#include "./lib/logging.hpp"
#include "subprojects/glm-1.0.1/glm/ext/vector_float3.hpp"

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

void InitializeViewport(GLFWwindow *, int, int);
void InitializeGlfw();
void InitializeGlad();
void setGlfwConfig();
void Cleanup();
void close_window_on_esc(GLFWwindow *);
unsigned int uniform_locator(GLuint, const GLchar *);

int main() {

    assert(logging::restart_gl_log());
    logging::gl_log("starting GLFW\n%s\n", glfwGetVersionString());

    InitializeGlfw();

    setGlfwConfig();

    GLFWwindow *window =
        glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Test Window", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    InitializeGlad();
    InitializeViewport(window, SCREENWIDTH, SCREENHEIGHT);

    float vertices[] = {
        0.0f, 1.0f,  0.0f, 0.5f,  0.0f,  0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.0f,
    };

    GLuint indices[]{0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1, 1, 4, 2, 2, 4, 3};

    GLuint vertex_buffer_object{};
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    GLuint vertex_array_object{};
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLuint element_buffer_object{};
    glGenBuffers(1, &element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const char *vertex_shader_source = R"(
#version 330 core

        layout(location = 0) in vec3 aPos;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position =
            projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }

    )";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    int compile_flag{};

    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        logging::log_shader_error(vertex_shader,
                                  "Error::Shader::Vertex::Compilation");
    }

    const char *fragment_shader_source = R"(
#version 330 core

out vec4 FragColor;
uniform vec4 ourColor;

void main()
{
    FragColor = ourColor;
}
    )";

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader_source, NULL);

    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        logging::log_shader_error(frag_shader,
                                  "Error::Shader::Fragment::Compilation");
    }

    GLuint shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);

    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_flag);

    if (!compile_flag) {
        logging::log_program_error(
            shader_program, "Linkage-Error::Shader::Fragment::Compilation");
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 view = glm::mat4(1.0f);
    float zoom_level = -3.0f;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, zoom_level));

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)SCREENWIDTH / (float)SCREENHEIGHT, 0.1f,
        100.0f);

    unsigned int model_loc = uniform_locator(shader_program, "model");
    unsigned int view_loc = uniform_locator(shader_program, "view");
    unsigned int projection_loc = uniform_locator(shader_program, "projection");

    glUseProgram(shader_program);

    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    while (!glfwWindowShouldClose(window)) {
        close_window_on_esc(window);

        glUseProgram(shader_program);
        glBindVertexArray(vertex_array_object);

        float currrent_time_frame = glfwGetTime();
        float rotation_angle = currrent_time_frame * 45.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation_angle),
                            glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

        float pyramid_blue_color = (sin(currrent_time_frame) / 2.0f) + 0.5f;
        int our_color_uniform_location =
            uniform_locator(shader_program, "ourColor");

        glUseProgram(shader_program);
        glUniform4f(our_color_uniform_location, 1, 0, pyramid_blue_color, 1.0f);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    Cleanup();
}

void InitializeGlfw() {
    if (glfwInit() != GLFW_TRUE) {
        logging::gl_log_err("GLFW Initialization Failed");
    }
}

void InitializeGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        logging::gl_log("Failed to Initialize GLAD");

        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
}

void InitializeViewport(GLFWwindow *window, int width, int height) {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void setGlfwConfig() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Cleanup() {
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/* first callback to close the window when ESC is pressed. */
void close_window_on_esc(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int uniform_locator(GLuint shader_program, const GLchar *name) {
    int uniform_located = glGetUniformLocation(shader_program, name);
    if (uniform_located == -1)
        logging::gl_log_err("Could not find uniform in shader");

    return uniform_located;
}