#ifndef __MESH_H__
#define __MESH_H__
#include "Shader.h"
#include <string>
#include <vector>
#include "Camera.h"
#include "vector/Vec2.hpp"

struct MVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 biTangent;
};

struct MTexture
{
    unsigned int id;
    std::string type;
    std::string path;

    void loadTexture(const char *path);

    bool loadTexture(const char *path, const std::string &directory, bool gamma = false);
};

typedef Shader MShader;

class Mesh
{
public:
    std::vector<MVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MTexture> textures;

    Mesh(const std::vector<MVertex> &vertices, const std::vector<unsigned int> &indices,
         const std::vector<MTexture> &textures, const Camera &camera);

    void render(MShader &shader, bool hasTexture = true);

     void renderMultipleTextures(MShader& shader);

    friend class Model;
private:
    unsigned int vao, vbo, ebo;

    const Camera *camera;

    void setupMesh();

};

#endif // __MESH_H__