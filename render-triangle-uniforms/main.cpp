#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <math.h>
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

    // You make context first then initialize Glad. How stupid I was.
    InitializeGlad();
    InitializeViewport(window, SCREENWIDTH, SCREENHEIGHT);

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

    // so that we can afford to call that bind to copy all the buffers
    // with glBufferData which has all the vertice data into memory.

    // the last argument of glBufferData determines how we manage the data
    // on the GPU
    // GL_STREAM_DRAW: the data is set only once and used by the GPU at
    // most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // we set the shader source with from vertex_shader.vs logic
    const char *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // We create the vertex shader object and set the source
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    // we set a flag and check if the shader was compiled successfully
    int compile_flag{};

    glCompileShader(vertex_shader);

    // sets the compile status for compiling the shader to our compile_flag
    // flag
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        log_shader_error(vertex_shader, "Error::Shader::Vertex::Compilation");
    }

    // We complete a similar process for the fragment shader
    const char *fragment_shader_source = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "uniform vec4 ourColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         " FragColor = ourColor;\n"
                                         "}\0";

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader_source, NULL);

    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_flag);

    if (!compile_flag) {
        log_shader_error(frag_shader, "Error::Shader::Fragment::Compilation");
    }

    // Creating Shader Programs
    GLuint shader_program = glCreateProgram();

    // We link the vertex shader output to the frag shader input
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

        // Iterate between the values of 1 to 0 when time passes.
        float time_variant = glfwGetTime();
        float green_value = (sin(time_variant) / 2.0f) + 0.5f;
        float red_value = (cos(time_variant) / 2.0f) + 0.5f;
        float blue_value = (sin(time_variant) / 4.0f) + 0.5f;

        std::cerr << "time: " << time_variant << "\tgreen: " << green_value
                  << "\red: " << red_value << "\n"
                  << "\tblue: " << blue_value << "\n";

        // Get uniform location from the shaders.
        int get_uniform_color_location =
            glGetUniformLocation(shader_program, "ourColor");

        if (get_uniform_color_location == -1)
            std::cerr << "could not find uniform";

        glUseProgram(shader_program);
        glUniform4f(get_uniform_color_location, red_value, green_value,
                    blue_value, 1.0f);

        glBindVertexArray(vertex_array_object);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    Cleanup();
}