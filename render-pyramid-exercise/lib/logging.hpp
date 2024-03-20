#include <cstdio>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <time.h>

#include "../lib/include/glad/glad.h"

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
