#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "VertexBufferObj.hpp"

class VAO {
public:
    GLuint ID;
    VAO();

    void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind();
    void Unbind();
    void Delete();
};