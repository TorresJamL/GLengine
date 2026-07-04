#version 330 core

out vec4 color;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTex;
uniform vec3 objColor;
uniform vec3 lightColor;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    color = texture(ourTex, TexCoord) * vec4(ambient, 1.0); // Mixes the texture color with our color
}