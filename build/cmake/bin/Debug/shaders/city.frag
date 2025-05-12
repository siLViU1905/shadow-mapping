#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 ViewPos;
in vec3 FragPos;
in mat3 TBN;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D texture_diffuse;
};

uniform Light light;
uniform Material material;

out vec4 FragColor;

void main() 
{
    FragColor = texture(material.texture_diffuse, TexCoords);
}