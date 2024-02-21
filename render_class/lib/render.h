#include <iostream>
#include <vector>

#include "../lib/include/glad/glad.h"
#include "../subprojects/glfw-3.3.9/include/GLFW/glfw3.h"

class VertexObject {
  private:
    GLuint vertex_buffer_object, vertex_array_object;

  public:
    VertexObject(float vertices[], unsigned long size_of_vertices,
                 GLuint vertex_buffer_object, GLuint vertex_array_object) {
        this->vertex_array_object = vertex_array_object;
        this->vertex_buffer_object = vertex_buffer_object;

        glGenVertexArrays(1, &vertex_array_object);
        glGenBuffers(1, &vertex_buffer_object);

        glBindVertexArray(vertex_array_object);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw() {
        glBindVertexArray(this->vertex_array_object);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

class ShaderObject {
  public:
    GLuint id{};
    unsigned int shader_type;

    ShaderObject(unsigned int shader_type) {
        this->shader_type = shader_type;
        this->id = glCreateShader(shader_type);
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

    void set_shader_source(const char *shader_source) {
        glShaderSource(this->id, 1, &shader_source, nullptr);
    }

    void compile() {
        int compile_flag{};

        glCompileShader(this->id);
        glGetShaderiv(this->id, GL_COMPILE_STATUS, &compile_flag);

        if (!compile_flag) {
            log_shader_error(this->id, "Error::Shader::Vertex::Compilation");
        }
    }
};

class ShaderProgramObject {
  private:
    GLuint shader_program{};

  public:
    ShaderProgramObject(std::vector<ShaderObject> shaders) {
        this->shader_program = glCreateProgram();

        for (auto &shader : shaders)
            glAttachShader(this->shader_program, shader.id);
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

    void compile_shader_program() {
        int compile_flag;

        glLinkProgram(this->shader_program);
        glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_flag);

        if (!compile_flag) {
            log_program_error(shader_program,
                              "Linkage-Error::Shader::Fragment::Compilation");
        }
    };

    void render() { glUseProgram(shader_program); }
};

class Renderer {

  private:
    GLFWwindow *window;
    int screen_width;
    int screen_height;
    const char *window_name;

  public:
    Renderer() {
        if (glfwInit() != GLFW_TRUE) {
            std::cerr << "Initialization failed.";
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void set_window_size(int width, int height) {
        screen_width = width;
        screen_height = height;
    }

    void set_window_name(const char *name) { window_name = name; }

    void close_renderer() {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    GLFWwindow *create_window() {
        window = glfwCreateWindow(this->screen_width, this->screen_height,
                                  this->window_name, nullptr, nullptr);

        if (window == nullptr) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);
        return window;
    }

    void initial_glad() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            this->close_renderer();
        }
    }

    void initial_viewport(GLFWwindow *window) {
        glfwGetFramebufferSize(window, &this->screen_width,
                               &this->screen_height);
        glViewport(0, 0, this->screen_width, this->screen_height);
    }

    void create_opengl_context(GLFWwindow *window) {
        glfwMakeContextCurrent(window);
    }

    void close_window_on_esc_callback(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    void render(GLFWwindow *window, ShaderProgramObject &shader_program,
                VertexObject &vertex_object) {

        while (!glfwWindowShouldClose(window)) {
            close_window_on_esc_callback(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader_program.render();
            vertex_object.draw();

            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }
};