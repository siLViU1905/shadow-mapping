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
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D texture_diffuse;
    float shininess;
};

uniform Light light[5];
uniform int lightCount;
uniform Material material;

vec3 processLight(int index, vec4 diffuseColor)
{
    vec3 finalNormal = normalize(Normal);

    float distance = length(light[index].position - FragPos);
    float attenuation = 1.0 / (light[index].constant + light[index].linear * distance + light[index].quadratic * (distance * distance));

    vec3 ambient = light[index].ambient * diffuseColor.rgb * 0.5;

    vec3 lightDir = normalize(light[index].position - FragPos);
    float diff = max(dot(finalNormal, lightDir), 0.0);
    vec3 diffuse = light[index].diffuse * light[index].color * diff * diffuseColor.rgb;

    diffuse *= attenuation;
    ambient *= attenuation;

    return diffuse + ambient;
}

out vec4 FragColor;

void main() {
    vec4 diffuseColor = texture(material.texture_diffuse, TexCoords);

    vec3 result = vec3(0.0);

    for(int i = 0; i < lightCount; i++)
     result += processLight(i, diffuseColor);

    FragColor = vec4(result, 1.0);
}