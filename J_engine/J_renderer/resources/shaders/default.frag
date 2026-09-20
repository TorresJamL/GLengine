#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;  

uniform Material material;
uniform Light light;
uniform sampler2D ourTex;
uniform vec3 objColor;
uniform vec3 viewPos;
uniform bool useTex = false;

void main() {
    // Ambient
    vec3 ambient = material.ambient * light.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diffuse_impact = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diffuse_impact * material.diffuse) * light.diffuse;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;
    
    vec3 result = (ambient + diffuse + specular);

    if (useTex) {
        color = texture(ourTex, TexCoord) * vec4(result, 1.0); // Mixes the texture color with our color    
    } else {
        color = vec4(1.0f, 0.0f, 0.0f, 1.0) * vec4(result, 1.0);
    }
}