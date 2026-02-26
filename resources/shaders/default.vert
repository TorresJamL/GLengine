#version 330 core
layout (location = 0) in vec3 aPos;

uniform float aspect;

void main(){
    vec3 scaledPos = aPos;
    scaledPos.x /= aspect;
    gl_Position = vec4(scaledPos, 1.0);
}