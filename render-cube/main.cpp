#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ratio>

#include "./lib/include/glad/glad.h"
#include "./subprojects/glfw-3.3.9/include/GLFW/glfw3.h"

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

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    const char *vertex_shader_source = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "void main()\n"
                                       "{\n"
                                       " gl_Position = vec4(aPos, 1.0);\n"
                                       "}\0";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    int compile_flag{};

    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        log_shader_error(vertex_shader, "Error::Shader::Vertex::Compilation");
    }

    const char *fragment_shader_source = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         " FragColor = vec4(1.0);\n"
                                         "}\0";

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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    Cleanup();
}