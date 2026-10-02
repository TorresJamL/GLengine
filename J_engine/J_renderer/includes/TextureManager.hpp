#pragma once

#include <iostream>
#include <unordered_map>
#include "Texture.hpp"

typedef unsigned short tex_unit;

class TextureManager {
public:
    const static struct TEX {
        const tex_unit MAX_UNITS = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
        tex_unit available = GL_TEXTURE0;
    };

    TextureManager::TEX tex;
    unordered_map<tex_unit, Texture> textureLookUpMap;

    void assign(Texture &t) {
        if (tex.available >= tex.MAX_UNITS) {
            throw TexUnitCapExceeded("Exceeded maximum amount of available texture units.");
        }
        textureLookUpMap[tex.available++] = t;
    }
    
    void remove(tex_unit texUnit) {
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

    bool lookUp(tex_unit unit) { return textureLookUpMap.contains(unit); }

    Texture& operator[] (const tex_unit key) {
        return textureLookUpMap[key];
    }
private:
protected:
};