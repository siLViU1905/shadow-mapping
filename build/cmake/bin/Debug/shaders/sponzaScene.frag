#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 ViewPos;
in vec3 FragPos;
in mat3 TBN;

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float cutoff;
    int type;
};

struct Material
{
    sampler2D texture_diffuse;
    sampler2D texture_normal;
};

uniform Light light[6];
uniform int lightCount;
uniform Material material;

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

    float currentDepth = projCoords.z;

    vec3 lightDir = normalize(light[0].type == 1 ? -light[0].direction : light[0].position - FragPos);
    float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

vec3 processLight(int index, vec4 diffuseColor, vec3 normal)
{
    float distance = length(light[index].position - FragPos) * 2.0;
    float attenuation = 1.0 / (light[index].constant + light[index].linear * distance + light[index].quadratic * (distance * distance));

    vec3 ambient = light[index].ambient * diffuseColor.rgb * 0.5;

    vec3 lightDir = normalize(light[index].position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light[index].diffuse * light[index].color * diff * diffuseColor.rgb;

    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = light[index].specular * spec * light[index].color;

    diffuse *= attenuation;
    ambient *= attenuation;
    specular *= attenuation;

    float shadow = 0.0;
    if(index == 0)
        shadow = calculateShadow(FragPosLightSpace);

    return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 processDirectionalLight(int index, vec4 diffuseColor, vec3 normal)
{
    vec3 ambient = light[index].ambient * diffuseColor.rgb * 0.5;

    vec3 lightDir = normalize(-light[index].direction);
    float diff = max(dot(normal, lightDir), 0.0);

    float theta = dot(normalize(FragPos - light[index].position), normalize(light[index].direction));

    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    if(theta > cos(light[index].cutoff))
    {

        diffuse = light[index].diffuse * light[index].color * diff * diffuseColor.rgb;

        vec3 viewDir = normalize(ViewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        specular = light[index].specular * spec * light[index].color;
    }

    return ambient + diffuse + specular;
}

out vec4 FragColor;

void main()
{

    vec4 diffuseColor = texture(material.texture_diffuse, TexCoords);

    vec3 normalMap = texture(material.texture_normal, TexCoords).rgb;
    normalMap = normalMap * 2.0 - 1.0;

    vec3 normal = normalize(TBN * normalMap);

    vec3 result = vec3(0.0);

    for(int i = 0; i < lightCount; i++)
    {
        if(light[i].type == 0)
            result += processLight(i, diffuseColor, normal);
        else if(light[i].type == 1)
            result += processDirectionalLight(i, diffuseColor, normal);
        else
        {
            result += processLight(i, diffuseColor, normal);
            result += processDirectionalLight(i, diffuseColor, normal);
        }
    }

    FragColor = vec4(result, diffuseColor.a);
}