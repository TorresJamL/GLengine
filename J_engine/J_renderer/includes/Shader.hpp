#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>

#include <glad.h>
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"
using namespace utilities;

std::string process_file(const char* filename);

typedef const char* cstr;

class Shader {
public:
    GLuint ID;

    Shader(cstr vertFilename, cstr fragFilename);

    void Use();
    void Delete();
    void ApplyAspectRatio(float aspect);
    
    GLuint getUniformLoc(const std::string &unif_name) const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    
private:
    void compileErrors(GLuint shader, cstr type);
};