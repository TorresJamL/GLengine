#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

std::string process_file(const char* filename);

typedef const char* cstr;

class Shader {
public:
    GLuint ID;

    Shader(cstr vertFilename, cstr fragFilename);

    void Use();
    void Delete();
private:
    void compileErrors(GLuint shader, cstr type);
};