#pragma once

#include <glad.h>
#include "GLFW/glfw3.h"

class VBO {
    public:
    GLuint ID;
    VBO(GLfloat* vertices, GLsizeiptr size);
    
    template <typename Container>
    VBO(const Container& vertices) {
        using T = typename Container::value_type;
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, 
            vertices.size() * sizeof(T), 
            vertices.data(), 
            GL_STATIC_DRAW);    
    }

    void Bind();
    void Unbind();
    void Delete();
};