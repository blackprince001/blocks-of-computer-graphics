/*
    Name = Appiah Boadu Prince Kwabena
    Index Number = 3024420
*/

#include <iostream>

#include "./lib/include/glad/glad.h"

#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"

#include "./subprojects//glm-1.0.1/glm/gtc/matrix_transform.hpp"
#include "./subprojects//glm-1.0.1/glm/gtc/type_ptr.hpp"
#include "./subprojects/glm-1.0.1/glm/glm.hpp"
#include "subprojects/glm-1.0.1/glm/ext/matrix_transform.hpp"
#include "subprojects/glm-1.0.1/glm/ext/vector_float3.hpp"

const int SCREENWIDTH = 1080;
const int SCREENHEIGHT = 720;

void intialize_viewport(GLFWwindow *, int, int);
void initialize_glfw();
void intialize_glad();
void set_glfw_config_variables();
void cleanup();
void close_window_on_esc_callback(GLFWwindow *);
unsigned int uniform_locator(GLuint, const GLchar *);

// Create enums for moves
enum Move { UP, DOWN, LEFT, RIGHT, SCALE_UP, SCALE_DOWN, INVALID };
Move handle_input_for_keys(GLFWwindow *window);

int main() {
    initialize_glfw();

    set_glfw_config_variables();

    GLFWwindow *window =
        glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Test Window", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    intialize_glad();
    intialize_viewport(window, SCREENWIDTH, SCREENHEIGHT);

    GLfloat vertices[]{-1.0f,  0.0f,  0.0f, -0.75f, 0.0f,  0.0f,
                       -0.75f, 0.25f, 0.0f, -1.0f,  0.0f,  0.0f,
                       -0.75f, 0.25f, 0.0f, -1.0f,  0.25f, 0.0f};

    GLuint vertex_array_object{};
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLuint vertex_buffer_object{};
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    const char *vertex_shader_source = R"(
    #version 330 core

    layout(location = 0) in vec3 coords;
    uniform mat4 trans;

    void main() {
        gl_Position = trans * vec4(coords, 1.0);
    }
)";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    int compile_flag{};

    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        std::cout << "Error::Shader::Vertex::Compilation";
    }

    const char *fragment_shader_source = R"(
    #version 330 core

    out vec4 FragColor;
    uniform vec3 changing_color;

    void main()
    {
        FragColor = vec4(changing_color, 1.0f);
    }
)";

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader_source, NULL);

    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        std::cout << "Error::Shader::Fragment::Compilation";
    }

    GLuint shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);

    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_flag);

    if (!compile_flag) {
        std::cout << "Linkage-Error::Shader::Fragment::Compilation";
    }

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader_program);

    glm::mat4 transformation = glm::mat4(1.0f);
    unsigned int trans_loc = uniform_locator(shader_program, "trans");

    GLfloat current_frame, delta_time, last_time;

    while (!glfwWindowShouldClose(window)) {
        close_window_on_esc_callback(window);

        current_frame = glfwGetTime();
        delta_time = current_frame - last_time;
        last_time = current_frame;

        Move move_key_pressed = handle_input_for_keys(window);

        if (move_key_pressed == Move::UP) {
            transformation = glm::translate(
                transformation, glm::vec3(0.0f, 0.5f, 0.0f) * delta_time);
        }
        if (move_key_pressed == Move::DOWN) {
            transformation = glm::translate(
                transformation, glm::vec3(0.0f, -0.5f, 0.0f) * delta_time);
        }
        if (move_key_pressed == Move::LEFT) {
            transformation = glm::translate(
                transformation, glm::vec3(-0.5f, 0.0f, 0.0f) * delta_time);
        }
        if (move_key_pressed == Move::RIGHT) {
            transformation = glm::translate(
                transformation, glm::vec3(0.5f, 0.0f, 0.0f) * delta_time);
        }
        if (move_key_pressed == Move::SCALE_UP) {
            transformation = glm::scale(
                transformation, glm::vec3(2.0f, 2.0f, 2.0f) * delta_time);
        }
        if (move_key_pressed == Move::SCALE_DOWN) {
            transformation = glm::scale(
                transformation, glm::vec3(0.05f, 0.05f, 0.05f) * delta_time);
        }

        // moves from 0.0 - 1.0 under the current time frame
        float changing_blue_color = (sin(current_frame) / 2.0f) + 0.5f;
        int changing_color_uniform =
            uniform_locator(shader_program, "changing_color");

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glUniformMatrix4fv(trans_loc, 1, GL_FALSE,
                           glm::value_ptr(transformation));
        glUniform4f(changing_color_uniform, 1.0, 1.0, changing_blue_color,
                    1.0f);

        glBindVertexArray(vertex_array_object);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);
    cleanup();
}

void initialize_glfw() {
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "GLFW Initialization Failed";
    }
}

void intialize_glad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to Initialize GLAD";

        cleanup();
    }
}

void intialize_viewport(GLFWwindow *window, int width, int height) {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void set_glfw_config_variables() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void cleanup() {
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}

void close_window_on_esc_callback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int uniform_locator(GLuint shader_program, const GLchar *name) {
    int uniform_located = glGetUniformLocation(shader_program, name);
    if (uniform_located == -1)
        std::cout << "Could not find uniform in shader";

    return uniform_located;
}

Move handle_input_for_keys(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        return Move::UP;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        return Move::LEFT;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        return Move::DOWN;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        return Move::RIGHT;
    else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        return Move::SCALE_UP;
    else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        return Move::SCALE_DOWN;

    return Move::INVALID;
}