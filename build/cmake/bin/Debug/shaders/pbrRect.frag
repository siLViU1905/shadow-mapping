#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 ViewPos;
in vec3 FragPos;
in mat3 TBN;
in vec3 WorldPos;

struct Light
{
    vec3 position;
    vec3 color;
    int type;
};

struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

uniform Light light[6];
uniform int lightCount;
uniform Material material;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

out vec4 FragColor;

in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;

float calculateShadow(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z < 0.0 || projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = 0.005;

    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(ViewPos - WorldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, material.albedo, material.metallic);

    vec3 Lo = vec3(0.0);

    float shadow = calculateShadow(FragPosLightSpace);

    for(int i = 0; i < lightCount; i++)
    {
        vec3 L = normalize(light[i].position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(light[i].position - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light[i].color * attenuation;

        float NDF = distributionGGX(N, H, material.roughness);
        float G = geometrySmith(N, V, L, material.roughness);
        vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;

        vec3 kD = vec3(1.0) - kS;

        kD *= 1.0 - material.metallic;

        float NdotL = max(dot(N, L), 0.0);

        if(i == 1)
            Lo += (1.0 - shadow) * (kD * material.albedo / PI + specular) * radiance * NdotL;
        else
            Lo += (kD * material.albedo / PI + specular) * radiance * NdotL;

    }

    vec3 ambient = vec3(0.03) * material.albedo * material.ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));

    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}