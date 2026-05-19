#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float aspect;
uniform mat4 transform;

void main(){
    if (aspect != 0.0f) {
        vec3 scaledPos = aPos;
        scaledPos.x /= aspect;
        gl_Position = transform * vec4(scaledPos, 1.0);
    } else {
        gl_Position = transform * vec4(aPos, 1.0f);
    }
    
    ourColor = aColor;
    TexCoord = aTexCoord;
}