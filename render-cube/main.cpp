#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ratio>

#include "./lib/include/glad/glad.h"
#include "./lib/shader.hpp"
#include "./subprojects//glm-1.0.0/glm/gtc/matrix_transform.hpp"
#include "./subprojects//glm-1.0.0/glm/gtc/type_ptr.hpp"
#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"
#include "./subprojects/glm-1.0.0/glm/glm.hpp"

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 600;

void InitializeGlfw() {
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Initialization failed.";
    }
}

void InitializeGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
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

void log_shader_error(const unsigned int shader,
                      const std::string &message = "") {
    constexpr unsigned int LOG_BUFFER_SIZE_BYTES{512};
    char log_info[LOG_BUFFER_SIZE_BYTES];

    glGetShaderInfoLog(shader, LOG_BUFFER_SIZE_BYTES, nullptr, log_info);

    if (!message.empty()) {
        std::cout << message << std::endl;
    }
    std::cout << log_info << std::endl;

    std::exit(EXIT_FAILURE);
}

void log_program_error(const unsigned int program,
                       const std::string &message = "") {
    constexpr unsigned int LOG_BUFFER_SIZE_BYTES{512};
    char log_info[LOG_BUFFER_SIZE_BYTES];

    glGetProgramInfoLog(program, LOG_BUFFER_SIZE_BYTES, nullptr, log_info);

    if (!message.empty()) {
        std::cout << message << std::endl;
    }
    std::cout << log_info << std::endl;

    std::exit(EXIT_FAILURE);
}

/* first callback to close the window when ESC is pressed. */
void close_window_on_esc(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
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
        // Front face
        -0.5f, -0.5f, 0.5f, // 0
        0.5f, -0.5f, 0.5f,  // 1
        0.5f, 0.5f, 0.5f,   // 2
        -0.5f, 0.5f, 0.5f,  // 3

        // Back face
        -0.5f, -0.5f, -0.5f, // 4
        0.5f, -0.5f, -0.5f,  // 5
        0.5f, 0.5f, -0.5f,   // 6
        -0.5f, 0.5f, -0.5f,  // 7

        // Left face
        -0.5f, -0.5f, -0.5f, // 8
        -0.5f, -0.5f, 0.5f,  // 9
        -0.5f, 0.5f, 0.5f,   // 10
        -0.5f, 0.5f, -0.5f,  // 11

        // Right face
        0.5f, -0.5f, 0.5f,  // 12
        0.5f, -0.5f, -0.5f, // 13
        0.5f, 0.5f, -0.5f,  // 14
        0.5f, 0.5f, 0.5f,   // 15

        // Top face
        -0.5f, 0.5f, 0.5f,  // 16
        0.5f, 0.5f, 0.5f,   // 17
        0.5f, 0.5f, -0.5f,  // 18
        -0.5f, 0.5f, -0.5f, // 19

        // Bottom face
        -0.5f, -0.5f, -0.5f, // 20
        0.5f, -0.5f, -0.5f,  // 21
        0.5f, -0.5f, 0.5f,   // 22
        -0.5f, -0.5f, 0.5f   // 23
    };
    unsigned int indices[] = {// Front face
                              0, 1, 2, 2, 3, 0,

                              // Back face
                              4, 5, 6, 6, 7, 4,

                              // Left face
                              8, 9, 10, 10, 11, 8,

                              // Right face
                              12, 13, 14, 14, 15, 12,

                              // Top face
                              16, 17, 18, 18, 19, 16,

                              // Bottom face
                              20, 21, 22, 22, 23, 20};

    GLuint VBO, cubeVAO, element_buffer_object;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &element_buffer_object);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    const char *vertex_shader_source = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;

void main()
{
        FragPos = vec3(model * vec4(aPos, 1.0));
        gl_Position = projection * view * model * vec4(aPos, 1.0);
}  
    )";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    int compile_flag{};

    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        log_shader_error(vertex_shader, "Error::Shader::Vertex::Compilation");
    }

    const char *fragment_shader_source = R"(
#version 330 core

in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 FragColor;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine ambient and diffuse
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader_source, NULL);

    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        log_shader_error(frag_shader, "Error::Shader::Fragment::Compilation");
    }

    GLuint shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);

    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_flag);

    if (!compile_flag) {
        log_program_error(shader_program,
                          "Linkage-Error::Shader::Fragment::Compilation");
    }

    while (!glfwWindowShouldClose(window)) {
        close_window_on_esc(window);

        // Clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind shader program and VAO
        glUseProgram(shader_program);
        glBindVertexArray(cubeVAO);

        // Update transformations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                800.0f / 600.0f, 0.1f, 100.0f);

        // Pass matrices to shaders
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1,
                           GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1,
                           GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"),
                           1, GL_FALSE, glm::value_ptr(projection));

        // Draw the cube
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    Cleanup();
}