#include "../lib/include/glad/glad.h"
#include "../subprojects/glfw-3.3.9/include/GLFW/glfw3.h"
#include <cassert>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <source_location>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <string_view>
#include <time.h>
#include <vector>

#define GL_LOG_FILE "gl.log"

namespace logging {

inline bool restart_gl_log(std::string_view log_file = GL_LOG_FILE) {
    FILE *file = std::fopen(std::string(log_file).c_str(), "w");
    if (!file) {
        std::fprintf(stderr,
                     "ERROR: could not open log file %.*s for writing\n",
                     static_cast<int>(log_file.size()), log_file.data());
        return false;
    }

    std::time_t now = std::time(nullptr);
    std::fprintf(file, "Log file %.*s. Local time %s\n",
                 static_cast<int>(log_file.size()), log_file.data(),
                 std::ctime(&now));

    std::fclose(file);
    return true;
}

inline bool gl_log(std::string_view message) {
    FILE *file = std::fopen(GL_LOG_FILE, "a");
    if (!file) {
        std::fprintf(stderr,
                     "ERROR: could not open GL_LOG_FILE file for appending\n");
        return false;
    }

    std::fprintf(file, "%.*s\n", static_cast<int>(message.size()),
                 message.data());
    std::fclose(file);
    return true;
}

inline bool gl_log_err(std::string_view message) {
    FILE *file = std::fopen(GL_LOG_FILE, "a");
    if (!file) {
        std::fprintf(stderr,
                     "ERROR: could not open GL_LOG_FILE file for appending\n");
        return false;
    }

    std::fprintf(file, "%.*s\n", static_cast<int>(message.size()),
                 message.data());
    std::fprintf(stderr, "%.*s\n", static_cast<int>(message.size()),
                 message.data());

    std::fclose(file);
    return true;
}

inline void log_shader_error(unsigned int shader,
                             std::string_view message = "") {
    constexpr unsigned int LOG_BUFFER_SIZE_BYTES = 512;
    std::vector<char> log_info(LOG_BUFFER_SIZE_BYTES);
    glGetShaderInfoLog(shader, LOG_BUFFER_SIZE_BYTES, nullptr, log_info.data());

    if (!message.empty()) {
        gl_log_err(message);
    }
    gl_log(log_info.data());
    std::exit(EXIT_FAILURE);
}

inline void log_program_error(unsigned int program,
                              std::string_view message = "") {
    constexpr unsigned int LOG_BUFFER_SIZE_BYTES = 512;
    std::vector<char> log_info(LOG_BUFFER_SIZE_BYTES);
    glGetProgramInfoLog(program, LOG_BUFFER_SIZE_BYTES, nullptr,
                        log_info.data());

    if (!message.empty()) {
        gl_log_err(message);
    }
    gl_log(log_info.data());
    std::exit(EXIT_FAILURE);
}

} // namespace logging

namespace utils {
enum ShaderType { VERTEX, FRAGMENT, PROGRAM };

inline std::string get_string_from_enum(ShaderType e) {
    switch (e) {
    case VERTEX:
        return "VERTEX";
    case FRAGMENT:
        return "FRAGMENT";
    case PROGRAM:
        return "PROGRAM";
    default:
        logging::gl_log_err("Failed to assign Shader Type");
        return "Failed";
    }
}
} // namespace utils

namespace engine {

class Shader {
  public:
    unsigned int shader_program_id;

    Shader(const char *vertexPath, const char *fragmentPath) {
        std::string vertex_code;
        std::string fragement_code;
        std::ifstream v_shader_file, f_shader_file;

        try {
            v_shader_file.open(vertexPath);
            f_shader_file.open(fragmentPath);

            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << v_shader_file.rdbuf();
            fShaderStream << f_shader_file.rdbuf();

            v_shader_file.close();
            f_shader_file.close();

            vertex_code = vShaderStream.str();
            fragement_code = fShaderStream.str();

        } catch (std::ifstream::failure &e) {
            logging::gl_log_err("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: ");
            logging::gl_log_err(e.what());
        }

        const char *vShaderCode = vertex_code.c_str();
        const char *fShaderCode = fragement_code.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        check_compile_errors(vertex,
                             utils::get_string_from_enum(utils::VERTEX));

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        check_compile_errors(fragment,
                             utils::get_string_from_enum(utils::FRAGMENT));

        shader_program_id = glCreateProgram();
        glAttachShader(shader_program_id, vertex);
        glAttachShader(shader_program_id, fragment);

        glLinkProgram(shader_program_id);
        check_compile_errors(shader_program_id,
                             utils::get_string_from_enum(utils::PROGRAM));

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() { glUseProgram(shader_program_id); }

  private:
    void check_compile_errors(GLuint shader, std::string type) {
        GLint success;

        if (type == utils::get_string_from_enum(utils::VERTEX) ||
            type == utils::get_string_from_enum(utils::FRAGMENT)) {

            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
                logging::log_shader_error(
                    shader, "Error::Shader::Fragment::Compilation");
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
                logging::log_program_error(
                    shader, "Linkage-Error::Shader::Fragment::Compilation");
        }
    }
};

class Renderer {

  private:
    GLFWwindow *window;
    int screen_width;
    int screen_height;
    const char *window_name;

  public:
    Renderer() {

        assert(logging::restart_gl_log());
        logging::gl_log("starting GLFW");

        if (glfwInit() != GLFW_TRUE) {
            logging::gl_log_err("GLFW Initialization Failed");
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

    void create_window() {
        window = glfwCreateWindow(screen_width, screen_height, window_name,
                                  nullptr, nullptr);
        if (window == nullptr) {
            logging::gl_log_err("Failed to create GLFW window");

            close_renderer();
        }
        logging::gl_log("OpenGL window created.");
    }

    void initial_glad() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            logging::gl_log_err("Failed to Initialize GLAD");
        }
        logging::gl_log("Glad Initialized.");

        close_renderer();
    }

    void initial_viewport() {
        glfwGetFramebufferSize(window, &screen_width, &screen_height);
        glViewport(0, 0, screen_width, screen_height);

        logging::gl_log("Viewport Set.");
    }

    void create_opengl_context() {
        glfwMakeContextCurrent(window);
        logging::gl_log("Successfully created OpenGL context.");
    }

    void close_window_on_esc_callback() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    void render(Shader shader_program, GLuint vertex_array_object) {
        while (!glfwWindowShouldClose(window)) {
            close_window_on_esc_callback();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader_program.use();
            glBindVertexArray(vertex_array_object);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }
};

} // namespace engine
