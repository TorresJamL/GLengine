#include "../includes/Texture.hpp" // Stupid darn GLM with their own Texture.hpp forcing me to use it's full legal name
using namespace std;
namespace fs = filesystem;

Texture::Texture() {}

void Texture::create2DTexture(string asset_file_path, bool flip_vertically_on_load, int desired_channels) {
    fs::path asset_fp(asset_file_path);
    if (!fs::exists(asset_fp)) {
        throw TextureError("Failed to load texture. Path does not exist.", asset_file_path.c_str());
    }
    this->asset_file_path = asset_file_path;
    this->texType = GL_TEXTURE_2D;

    glGenTextures(1, &ID);
    glBindTexture(this->texType, ID);

    stbi_set_flip_vertically_on_load(flip_vertically_on_load); // stbi load flipped bcuz of gl coords
    data = stbi_load(asset_file_path.c_str(), &width, &height, &nrChannels, desired_channels);
    if (data) {
        GLenum format;
        if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
            
        glTexImage2D(this->texType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(this->texType);
    } else {
        throw TextureError("Failed to load texture", asset_file_path.c_str());
    }
    data_was_freed = false;
}

unsigned char* extractFace(const unsigned char* src_img, int w, int xOffset, int yOffset, int faceSize, int channels) {
    unsigned char* face = new unsigned char[faceSize * faceSize * channels];
    for (int y = 0; y < faceSize; ++y) {
        for (int x = 0; x < faceSize; ++x) {
            int srcX = xOffset + x;
            int srcY = yOffset + y;
            int srcIndex = (srcY * w + srcX) * channels;
            int dstIndex = (y * faceSize + x) * channels;
            for (int c = 0; c < channels; ++c) {
                face[dstIndex + c] = src_img[srcIndex + c];
            }
        }
    }
    return face;
}

void Texture::createCubeMapTexture(string asset_file_path, bool isLandscapeOriented, int desired_channels) {
    this->asset_file_path = asset_file_path;
    this->texType = GL_TEXTURE_CUBE_MAP;

    glGenTextures(1, &ID);
    glBindTexture(this->texType, ID);

    int width, height, nrChannels;
    data = stbi_load(asset_file_path.c_str(), &width, &height, &nrChannels, desired_channels);
    if (!data) throw TextureError("Failed to load texture", asset_file_path.c_str());

    int channels = desired_channels != 0 ? desired_channels : nrChannels;
    int faceSize = (isLandscapeOriented) ? width / 4 : width / 3;

    vector<unsigned char*> faces = {
        extractFace(data, width, 2 * faceSize, faceSize, faceSize, channels), // +X
        extractFace(data, width, 0, faceSize, faceSize, channels),            // -X
        extractFace(data, width, faceSize, 0, faceSize, channels),            // +Y
        extractFace(data, width, faceSize, 2 * faceSize, faceSize, channels), // -Y
        extractFace(data, width, faceSize, faceSize, faceSize, channels),     // +Z
        extractFace(data, width, 3 * faceSize, faceSize, faceSize, channels)  // -Z
    };

    GLenum format;
    if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;
    for (GLuint i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, faceSize, faceSize, 0, format, GL_UNSIGNED_BYTE, faces[i]);
        delete[] faces[i];
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    data_was_freed = false;
}

void Texture::Bind() {
    glBindTexture(texType, ID);
}

void Texture::Unbind() {
    glBindTexture(texType, 0);
}

void Texture::setTexParamInt(GLenum param_name, GLint param) {
    glTexParameteri(texType, param_name, param);
}

void Texture::setTexParamFloat(GLenum param_name, GLfloat param) {
    glTexParameterf(texType, param_name, param);
}

void Texture::Free() {
    data_was_freed = true;
    stbi_image_free(data);
}