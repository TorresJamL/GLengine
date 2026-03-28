#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float aspect;

void main(){
    vec3 scaledPos = aPos;
    scaledPos.x /= aspect;
    gl_Position = vec4(scaledPos, 1.0);

    ourColor = aColor;
    TexCoord = aTexCoord;
}