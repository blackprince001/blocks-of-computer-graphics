#include "../lib/include/glad/glad.h"
#include "../subprojects/glfw-3.3.9/include/GLFW/glfw3.h"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <string_view>
#include <time.h>

#define GL_LOG_FILE "gl.log"

namespace logging {
inline bool restart_gl_log() {
    std::FILE *file = fopen(GL_LOG_FILE, "w");
    {
        if (!file) {
            fprintf(
                stderr,
                "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
                GL_LOG_FILE);
            return false;
        }
        time_t now = time(NULL);
        char *date = ctime(&now);
        fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
    }

    return true;
}

inline bool gl_log(const char *message, ...) {
    va_list argptr;
    std::FILE *file = fopen(GL_LOG_FILE, "a");
    {
        if (!file) {
            fprintf(stderr,
                    "ERROR: could not open GL_LOG_FILE %s file for appending\n",
                    GL_LOG_FILE);
            return false;
        }

        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);
    }

    return true;
}

inline bool gl_log_err(const char *message, ...) {
    va_list argptr;
    std::FILE *file = fopen(GL_LOG_FILE, "a");
    {
        if (!file) {
            fprintf(stderr,
                    "ERROR: could not open GL_LOG_FILE %s file for appending\n",
                    GL_LOG_FILE);
            return false;
        }
        va_start(argptr, message);
        vfprintf(file, message, argptr);
        va_end(argptr);
        va_start(argptr, message);
        vfprintf(stderr, message, argptr);
        va_end(argptr);
    }

    return true;
}

inline void log_shader_error(const unsigned int shader,
                             const std::string &message = "") {
    constexpr unsigned int LOG_BUFFER_SIZE_BYTES{512};
    char log_info[LOG_BUFFER_SIZE_BYTES];

    glGetShaderInfoLog(shader, LOG_BUFFER_SIZE_BYTES, nullptr, log_info);

    if (!message.empty()) {
        logging::gl_log_err(message.c_str());
    }

    logging::gl_log(log_info);

    std::exit(EXIT_FAILURE);
}

inline void log_program_error(const unsigned int program,
                              const std::string &message = "") {
    constexpr unsigned int LOG_BUFFER_SIZE_BYTES{512};
    char log_info[LOG_BUFFER_SIZE_BYTES];

    glGetProgramInfoLog(program, LOG_BUFFER_SIZE_BYTES, nullptr, log_info);

    if (!message.empty()) {
        logging::gl_log_err(message.c_str());
    }

    logging::gl_log(log_info);

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
class Renderer {

  private:
    GLFWwindow *window;
    int screen_width;
    int screen_height;
    const char *window_name;

  public:
    Renderer() {

        assert(logging::restart_gl_log());
        logging::gl_log("starting GLFW\n%s\n", glfwGetVersionString());

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

    GLFWwindow *create_window() {
        window = glfwCreateWindow(screen_width, screen_height, window_name,
                                  nullptr, nullptr);

        if (window == nullptr) {
            logging::gl_log_err("Failed to create GLFW window");

            close_renderer();
        }
        return window;
    }

    void initial_glad() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            logging::gl_log("Failed to Initialize GLAD");
        }

        close_renderer();
    }

    void initial_viewport(GLFWwindow *window) {
        glfwGetFramebufferSize(window, &screen_width, &screen_height);
        glViewport(0, 0, screen_width, screen_height);
    }

    void create_opengl_context(GLFWwindow *window) {
        glfwMakeContextCurrent(window);
    }

    void close_window_on_esc_callback(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
};

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
} // namespace engine
