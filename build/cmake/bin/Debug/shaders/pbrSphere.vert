#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBiTangent;

out vec2 TexCoords;
out vec3 Normal;
out vec3 ViewPos;
out vec3 FragPos;
out mat3 TBN;
out vec3 WorldPos;

struct Camera {
    mat4 view;
    mat4 projection;
    vec3 position;
};

uniform Camera camera;
uniform mat4 model;

uniform mat4 lightSpaceMatrix;
out vec4 FragPosLightSpace;

void main() {
    WorldPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = camera.projection * camera.view * vec4(WorldPos, 1.0);
    
    TexCoords = aTexCoord;
    
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    
    ViewPos = camera.position;
    
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBiTangent);
    vec3 N = normalize(mat3(model) * aNormal);
    
    TBN = mat3(T, B, N);

    FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos,1.0);
}