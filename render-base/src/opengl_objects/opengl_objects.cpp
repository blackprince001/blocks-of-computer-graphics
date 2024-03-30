#include "../../include/opengl_objects/opengl_objects.h"

gl_object::VBO::VBO(GLfloat *vertices, GLsizeiptr size) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void gl_object::VBO::bind_vbo() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
void gl_object::VBO::unbind_vbo() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
void gl_object::VBO::delete_vbo() { glDeleteBuffers(1, &ID); }

gl_object::VAO::VAO() { glGenVertexArrays(1, &ID); }

void gl_object::VAO::link_vbo(VBO &VBO, GLuint layout) {
  VBO.bind_vbo();
  glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(layout);

  VBO.unbind_vbo();
}

void gl_object::VAO::bind_vao() { glBindVertexArray(ID); }
void gl_object::VAO::unbind_vao() { glBindVertexArray(0); }
void gl_object::VAO::delete_vao() { glDeleteVertexArrays(1, &ID); }

gl_object::EBO::EBO(GLuint *indices, GLsizeiptr size) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void gl_object::EBO::bind_ebo() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
void gl_object::EBO::unbind_ebo() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
void gl_object::EBO::delete_ebo() { glDeleteBuffers(1, &ID); }