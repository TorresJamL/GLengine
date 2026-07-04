#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>

#include <glad.h>
#include "GLFW/glfw3.h"

#include <stb_image.h>

#include "utils.hpp"

using namespace std;

class Texture {
public:
    const static int maxTexUnits = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
    GLuint ID;
    GLenum texType;
    string asset_file_path; // File directory

    // Texture(string asset_file_path, GLenum texType, bool flip_vertically_on_load, int desired_channels);
    Texture();

    // void GenTextures(GLsizei n, GLuint* textures);
    void Bind();
    void Unbind();
    void setTexParamInt(GLenum param_name, GLint param);
    void setTexParamFloat(GLenum param_name, GLfloat param);

    void storeAssetData() {
        throw NotImplemented();
    }
    /** Functionality might require new blank constructor. */
    void restoreAssetData() {
        throw NotImplemented();
    }

    void create2DTexture(string asset_file_path, bool flip_vertically_on_load = true, int desired_channels = 0);
    void createCubeMapTexture(string asset_file_path, bool flip_vertically_on_load, int desired_channels);

    void Free();
private:
    unsigned char *data;
    int width, height, nrChannels;
    bool data_was_freed = false; 
};