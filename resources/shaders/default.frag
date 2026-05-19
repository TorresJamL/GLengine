#version 330 core

out vec4 color;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTex;

void main(){
    color = texture(ourTex, TexCoord); // * vec4(ourColor, 1.0); // Mixes the texture color with our color
}