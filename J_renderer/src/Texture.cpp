#include "../includes/Texture.hpp" // Stupid darn GLM with their own Texture.hpp forcing me to use it's full legal name
using namespace std;

// Texture::Texture(string asset_file_path, GLenum texType, bool flip_vertically_on_load, int desired_channels) {
//     this->asset_file_path = asset_file_path;
//     this->texType = texType;

//     glGenTextures(1, &ID);
//     glBindTexture(texType, ID);

//     stbi_set_flip_vertically_on_load(flip_vertically_on_load); // stbi load flipped bcuz of gl coords
//     data = stbi_load(asset_file_path.c_str(), &width, &height, &nrChannels, desired_channels);
//     if (data) {
//         GLenum format;
//         if (nrChannels == 3)
//             format = GL_RGB;
//         else if (nrChannels == 4)
//             format = GL_RGBA;
//         glTexImage2D(texType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(texType);
//     } else {
//         throw TextureError("Failed to load texture", asset_file_path.c_str());
//     }
//     data_was_freed = false;
// }

Texture::Texture(/*string asset_file_path, GLenum texType*/) {
    // this->asset_file_path = asset_file_path;
    // this->texType = texType;
    // bool flip_vertically_on_load = true;
    // int desired_channels = 0;

    // glGenTextures(1, &ID);
    // glBindTexture(texType, ID);

    // stbi_set_flip_vertically_on_load(flip_vertically_on_load); // stbi load flipped bcuz of gl coords
    // data = stbi_load(asset_file_path.c_str(), &width, &height, &nrChannels, desired_channels);
    // if (data) {
    //     GLenum format;
    //     if (nrChannels == 3)
    //         format = GL_RGB;
    //     else if (nrChannels == 4)
    //         format = GL_RGBA;
    //     glTexImage2D(texType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(texType);
    // } else {
    //     throw TextureError("Failed to load texture", asset_file_path.c_str());
    // }
    // data_was_freed = false;
}

void Texture::create2DTexture(string asset_file_path, bool flip_vertically_on_load, int desired_channels) {
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

void Texture::createCubeMapTexture(string asset_file_path, bool flip_vertically_on_load, int desired_channels) {
    this->asset_file_path = asset_file_path;
    this->texType = GL_TEXTURE_CUBE_MAP;

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
    } else {
        throw TextureError("Failed to load texture", asset_file_path.c_str());
    }
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