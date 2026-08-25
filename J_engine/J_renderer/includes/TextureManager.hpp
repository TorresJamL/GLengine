#pragma once

#include <iostream>
#include <unordered_map>
#include "Texture.hpp"

class TextureManager {
public:
    const static struct TEX {
        const unsigned short MAX_UNITS = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
        unsigned short available = GL_TEXTURE0;
    };

    TextureManager::TEX tex;
    unordered_map<unsigned short, Texture> textureLookUpMap;

    void assign(Texture &t) {
        if (tex.available >= tex.MAX_UNITS) {
            throw TexUnitCapExceeded("Exceeded maximum amount of available texture units.");
        }
        textureLookUpMap[tex.available++] = t;
    }
    
    void remove(unsigned short texUnit) {
        textureLookUpMap.erase(texUnit);
    } 

    bool lookUp(Texture& t) { 
        for (auto it = textureLookUpMap.begin(); it != textureLookUpMap.end(); ++it) {
            if (it->second == t) {
                return true;
            }
        }
        return false;
    }

    bool lookUp(unsigned short unit) { return textureLookUpMap.contains(unit); }

    Texture& operator[] (const unsigned short key) {
        return textureLookUpMap[key];
    }
private:
protected:
};