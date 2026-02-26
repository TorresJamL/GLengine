#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>

#include <glad.h>
#include "GLFW/glfw3.h"

std::string process_file(const char* filename);

typedef const char* cstr;

class Shader {
public:
    GLuint ID;

    Shader(cstr vertFilename, cstr fragFilename);

    void Use();
    void Delete();
    void ApplyAspectRatio(float aspect);
private:
    void compileErrors(GLuint shader, cstr type);
};