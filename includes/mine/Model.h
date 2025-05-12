#ifndef __MODEL_H__
#define __MODEL_H__
#include "Mesh.h"
#include "../assimp/Importer.hpp"
#include "../assimp/scene.h"
#include "../assimp/postprocess.h"
#include <unordered_map>

class Model
{
   std::vector<Mesh> meshes;
   std::string directory;

   std::unordered_map<std::string, MTexture> loadedTextures;

   void loadModel(const std::string& path);

   void proccesNode(aiNode* node, const aiScene* scene);

   Mesh proccesMesh(aiMesh* mesh, const aiScene* scene);

   std::vector<MTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
 
   const Camera* camera;

   static std::unordered_map<int, std::string> texturesMap;

   static void initTexturesMap();

   glm::mat4 model;

   bool multiple_textures;

   public:
   Model();

   Model(const char* path, const Camera& camera, bool multiple_textures = false);

   Model(const std::vector<Mesh>& meshes, const Camera& camera);

   glm::vec3 position;
   glm::vec3 scale;
   glm::vec3 rotation;
   glm::vec3 angles;

   bool translate_before_rotation;
   
    glm::mat4& getModel();

   void render(MShader& shader, bool hasTexture = true);
};

#endif // __MODEL_H__