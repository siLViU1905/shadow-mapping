#include "Model.h"
#include <stdexcept>
#include <iostream>
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

void Model::render(MShader &shader, bool hasTexture)
{
    model = glm::mat4(1.f);
    if (translate_before_rotation)
    {
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(angles.x), glm::vec3(rotation.x, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.f, rotation.y, 0.f));
        model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.f, 0.f, rotation.z));
    }
    else
    {
        model = glm::rotate(model, glm::radians(angles.x), glm::vec3(rotation.x, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.f, rotation.y, 0.f));
        model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.f, 0.f, rotation.z));
        model = glm::translate(model, position);
    }
   
    model = glm::scale(model, scale);

    shader.setMat4("camera.view", camera->getView());
    shader.setMat4("camera.projection", camera->getProjection());
    shader.setVec3("camera.position", camera->getPosition());

    shader.setMat4("model", model);

    if (!multiple_textures)
        for (Mesh &mesh : meshes)
            mesh.render(shader, hasTexture);
    else
        for (Mesh &mesh : meshes)
            mesh.renderMultipleTextures(shader);
}

void Model::loadModel(const std::string &path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |
                                                     aiProcess_GenSmoothNormals |
                                                     aiProcess_FlipUVs |
                                                     aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw std::runtime_error(std::string("ERROR::ASSIMP::") + import.GetErrorString());

    directory = path.substr(0, path.find_last_of('/'));

    proccesNode(scene->mRootNode, scene);
}

void Model::proccesNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(proccesMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
        proccesNode(node->mChildren[i], scene);
}

Mesh Model::proccesMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<MVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MTexture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        MVertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;

            if (mesh->HasTangentsAndBitangents())
            {
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.biTangent = vector;
            }
        }
        else
            vertex.texCoords = glm::vec2(0.f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        const aiTextureType types[] = {
            aiTextureType_DIFFUSE,
            aiTextureType_SPECULAR,
            aiTextureType_AMBIENT,
            aiTextureType_EMISSIVE,
            aiTextureType_HEIGHT,
            aiTextureType_NORMALS,
            aiTextureType_SHININESS,
            aiTextureType_OPACITY,
            aiTextureType_DISPLACEMENT,
            aiTextureType_LIGHTMAP,
            aiTextureType_REFLECTION,
            aiTextureType_BASE_COLOR,
            aiTextureType_NORMAL_CAMERA,
            aiTextureType_EMISSION_COLOR,
            aiTextureType_METALNESS,
            aiTextureType_DIFFUSE_ROUGHNESS,
            aiTextureType_AMBIENT_OCCLUSION};

        /*for (const auto &type : types)
        {
            unsigned int count = material->GetTextureCount(type);
             std::cout << "Texture type " << type << " count: " << count << std::endl;
        }*/

        /*std::vector<MTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

         textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

         std::vector<MTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

         textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());*/

        for (const auto &type : types)
        {
            auto texs = loadMaterialTextures(material, type, "");
            textures.insert(textures.end(), texs.begin(), texs.end());
        }
    }

    return Mesh(vertices, indices, textures, *camera);
}

std::vector<MTexture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName)
{
    std::vector<MTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texturePath = str.C_Str();

        bool skip = false;

        auto lTex = loadedTextures.find(texturePath.c_str());

        if (lTex != loadedTextures.end())
        {
            textures.push_back(lTex->second);
            skip = true;
        }

        if (!skip)
        {
            MTexture texture;

            std::string fullPath = directory + texturePath;
            std::string filename = texturePath;

            size_t lastSlash = texturePath.find_last_of("/\\");
            if (lastSlash != std::string::npos)
                filename = texturePath.substr(lastSlash + 1);

            if (!texture.loadTexture(texturePath.c_str(), directory))
                throw std::runtime_error("Texture not found");

            texture.type = texturesMap[type];
            std::cout << "Texture loaded: " << texture.type << '\n';
            texture.path = texturePath;
            textures.push_back(texture);
            loadedTextures[texturePath] = texture;
        }
    }
    return textures;
}

std::unordered_map<int, std::string> Model::texturesMap;

void Model::initTexturesMap()
{
    texturesMap[aiTextureType_DIFFUSE] = "texture_diffuse";
    texturesMap[aiTextureType_NORMALS] = "texture_normal";
    texturesMap[aiTextureType_LIGHTMAP] = "texture_lightmap";
    texturesMap[aiTextureType_BASE_COLOR] = "texture_basecolor";
    texturesMap[aiTextureType_NORMAL_CAMERA] = "texture_normal_camera";
    texturesMap[aiTextureType_EMISSION_COLOR] = "texture_emission";
    texturesMap[aiTextureType_SPECULAR] = "texture_specular";
    texturesMap[aiTextureType_AMBIENT] = "texture_ambient";
    texturesMap[aiTextureType_EMISSIVE] = "texture_emissive";
    texturesMap[aiTextureType_HEIGHT] = "texture_height";
    texturesMap[aiTextureType_SHININESS] = "texture_shininess";
    texturesMap[aiTextureType_OPACITY] = "texture_opacity";
    texturesMap[aiTextureType_DISPLACEMENT] = "texture_displacement";
    texturesMap[aiTextureType_REFLECTION] = "texture_reflection";
    texturesMap[aiTextureType_METALNESS] = "texture_metalness";
    texturesMap[aiTextureType_AMBIENT_OCCLUSION] = "texture_ao";
}

Model::Model()
{
}

Model::Model(const char *path, const Camera &camera, bool multiple_textures)
{
    this->camera = &camera;
    this->multiple_textures = multiple_textures;
    model = glm::mat4(1.f);
    position = glm::vec3(0.f);
    rotation = scale = glm::vec3(1.f);
    angles = glm::vec3(0.f);
    translate_before_rotation = true;
    initTexturesMap();
    loadModel(path);
}

Model::Model(const std::vector<Mesh>& meshes, const Camera& camera)
{
    this->camera = &camera;
    model = glm::mat4(1.f);
    position = glm::vec3(0.f);
    rotation = scale = glm::vec3(1.f);
    angles = glm::vec3(0.f);
    translate_before_rotation = true;
   this->meshes = meshes;
}

 glm::mat4& Model::getModel()
{
    model = glm::mat4(1.f);
    if (translate_before_rotation)
    {
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(angles.x), glm::vec3(rotation.x, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.f, rotation.y, 0.f));
        model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.f, 0.f, rotation.z));
    }
    else
    {
        model = glm::rotate(model, glm::radians(angles.x), glm::vec3(rotation.x, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.f, rotation.y, 0.f));
        model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.f, 0.f, rotation.z));
        model = glm::translate(model, position);
    }
   
    model = glm::scale(model, scale);

    return model;
}
