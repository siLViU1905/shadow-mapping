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
    sampler2D texture_specular;
    sampler2D texture_height;
    
    float shininess;
};

uniform Light light;
uniform Material material;

out vec4 FragColor;

void main() {
    vec4 diffuseColor = texture(material.texture_diffuse, TexCoords);
    vec4 specularColor = texture(material.texture_specular, TexCoords);
    float height = texture(material.texture_height, TexCoords).r;
    
    vec3 finalNormal = normalize(Normal);
    
    vec3 viewDir = normalize(ViewPos - FragPos);
    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 ambient = light.ambient * diffuseColor.rgb * 0.5;
    
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(finalNormal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseColor.rgb;
    
    vec3 reflectDir = reflect(-lightDir, finalNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularColor.rgb;
    

    float heightFactor = height * 0.2;
    diffuse *= (1.0 - heightFactor);
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    vec3 result = ambient + diffuse + specular;
    
    FragColor = diffuseColor;
}