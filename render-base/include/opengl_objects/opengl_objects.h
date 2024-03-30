#ifndef GL_Object_CLASS_H
#define GL_Object_CLASS_H

#include "../glad/glad.h"

namespace gl_object {
class VBO {
  public:
    GLuint ID;

    VBO(GLfloat *vertices, GLsizeiptr size);

    void bind_vbo();
    void unbind_vbo();
    void delete_vbo();
};

class VAO {
  public:
    GLuint ID;
    VAO();

    // should probably set up the stride too
    void link_vbo(VBO &VBO, GLuint layout);
    void bind_vao();
    void unbind_vao();
    void delete_vao();
};

class EBO {
  public:
    GLuint ID;
    EBO(GLuint *indices, GLsizeiptr size);

    void bind_ebo();
    void unbind_ebo();
    void delete_ebo();
};
} // namespace gl_object

#endif