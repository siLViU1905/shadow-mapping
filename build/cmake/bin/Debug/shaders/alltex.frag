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
    sampler2D texture_normal;
    sampler2D texture_lightmap;
    sampler2D texture_basecolor;
    sampler2D texture_normal_camera;
    sampler2D texture_emission;
    
    sampler2D texture_specular;
    sampler2D texture_ambient;
    sampler2D texture_emissive;
    sampler2D texture_height;
    sampler2D texture_shininess;
    sampler2D texture_opacity;
    sampler2D texture_displacement;
    sampler2D texture_reflection;
    sampler2D texture_metalness;
    sampler2D texture_roughness;
    sampler2D texture_ao;
    
    float shininess;
};

uniform Light light;
uniform Material material;

out vec4 FragColor;

void main() {
    vec4 diffuseColor = texture(material.texture_diffuse, TexCoords);
    vec4 baseColor = texture(material.texture_basecolor, TexCoords);
    vec4 emissionColor = texture(material.texture_emission, TexCoords);
    vec4 lightmapColor = texture(material.texture_lightmap, TexCoords);
    
    vec4 specularColor = texture(material.texture_specular, TexCoords);
    vec4 ambientOcclusion = texture(material.texture_ao, TexCoords);
    vec4 roughnessMap = texture(material.texture_roughness, TexCoords);
    vec4 metalnessMap = texture(material.texture_metalness, TexCoords);
    
    vec3 normal = texture(material.texture_normal, TexCoords).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    
    vec3 normalCamera = texture(material.texture_normal_camera, TexCoords).rgb;
    normalCamera = normalCamera * 2.0 - 1.0;
    
    vec3 finalNormal = normal;
    
    vec3 viewDir = normalize(ViewPos - FragPos);
    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 ambient = light.ambient * mix(diffuseColor.rgb, baseColor.rgb, 0.5);
    
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(finalNormal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * mix(diffuseColor.rgb, baseColor.rgb, 0.5);
    
    vec3 reflectDir = reflect(-lightDir, finalNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularColor.rgb;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    vec3 lightmapContribution = lightmapColor.rgb * diffuseColor.rgb;
    
    vec3 emission = emissionColor.rgb * emissionColor.a;
    
    float ao = ambientOcclusion.r;
    
    float metalness = metalnessMap.r;
    float roughness = roughnessMap.r;
    
    if (metalness > 0.5) {
        specular *= 2.0 * metalness;
    }
    
    if (roughness > 0.5) {
        specular *= (1.0 - roughness * 0.7);
    }
    
    vec3 result = (ambient * ao) + diffuse + specular + lightmapContribution + emission;
    
    FragColor = vec4(result, diffuseColor.a);
}