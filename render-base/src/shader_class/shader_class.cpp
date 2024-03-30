#include "../../include/shader_class/shader_class.h"

std::string get_file_contents(const char *filename) {
    std::ifstream in(filename, std::ios::binary);

    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw(errno);
}

std::string get_string_from_enum(ShaderType e) {
    switch (e) {
    case VERTEX:
        return "VERTEX";
    case FRAGMENT:
        return "FRAGMENT";
    case PROGRAM:
        return "PROGRAM";
    default:
        return "Failed";
    }
}

engine::Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    std::string vertex_code = get_file_contents(vertexFile);
    std::string fragement_code = get_file_contents(fragmentFile);

    const char *vertexSource = vertex_code.c_str();
    const char *fragmentSource = fragement_code.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexSource, NULL);
    glCompileShader(vertex_shader);
    check_compile_errors(vertex_shader, get_string_from_enum(VERTEX));

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentSource, NULL);
    glCompileShader(fragment_shader);
    check_compile_errors(fragment_shader, get_string_from_enum(FRAGMENT));

    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    check_compile_errors(ID, get_string_from_enum(PROGRAM));

    glLinkProgram(ID);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void engine::Shader::check_compile_errors(GLuint shader, std::string type) {
    GLint success{};

    if (type == get_string_from_enum(VERTEX) ||
        type == get_string_from_enum(FRAGMENT)) {

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
            std::cout << "Error::Shader::Fragment::Compilation" << std::endl;
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
            std::cout << "Linkage-Error::Shader::Fragment::Compilation"
                      << std::endl;
    }
}
void engine::Shader::use_shader_program() { glUseProgram(ID); }
void engine::Shader::delete_shader_program() { glDeleteProgram(ID); }
