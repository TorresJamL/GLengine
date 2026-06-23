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
#include <glm/gtx/string_cast.hpp>

#include "utils.hpp"

using namespace std;

namespace G_Framework {
    typedef struct {
        glm::vec3 up;
    } Locals;
    
    class Shape3D {
    public:
        glm::vec3 origin;

        int scalar;
    };
}