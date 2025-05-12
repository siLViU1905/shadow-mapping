#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>
#include <iostream>

Mesh::Mesh(const std::vector<MVertex> &vertices, const std::vector<unsigned int> &indices,
           const std::vector<MTexture> &textures, const Camera &camera) : vertices(vertices), indices(indices), textures(textures)
{
    this->camera = &camera;
    setupMesh();
}

void Mesh::render(MShader &shader, bool hasTexture)
{
    if (hasTexture)
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string uniformName = "material." + textures[i].type;

            shader.setInt(uniformName.c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::renderMultipleTextures(MShader &shader)
{
    // Counters for each texture type
    unsigned int diffuseNr = 1;
    unsigned int normalNr = 1;
    unsigned int lightmapNr = 1;
    unsigned int basecolorNr = 1;
    unsigned int normalCameraNr = 1;
    unsigned int emissionNr = 1;
    unsigned int specularNr = 1;
    unsigned int ambientNr = 1;
    unsigned int emissiveNr = 1;
    unsigned int heightNr = 1;
    unsigned int shininessNr = 1;
    unsigned int opacityNr = 1;
    unsigned int displacementNr = 1;
    unsigned int reflectionNr = 1;
    unsigned int metalnessNr = 1;
    unsigned int aoNr = 1;

    // Bind each texture with the appropriate counter
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        // Activate texture unit
        glActiveTexture(GL_TEXTURE0 + i);

        // Get texture info
        std::string name = textures[i].type;
        std::string number;

        // Determine which counter to use based on texture type
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_lightmap")
            number = std::to_string(lightmapNr++);
        else if (name == "texture_basecolor")
            number = std::to_string(basecolorNr++);
        else if (name == "texture_normal_camera")
            number = std::to_string(normalCameraNr++);
        else if (name == "texture_emission")
            number = std::to_string(emissionNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_ambient")
            number = std::to_string(ambientNr++);
        else if (name == "texture_emissive")
            number = std::to_string(emissiveNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);
        else if (name == "texture_shininess")
            number = std::to_string(shininessNr++);
        else if (name == "texture_opacity")
            number = std::to_string(opacityNr++);
        else if (name == "texture_displacement")
            number = std::to_string(displacementNr++);
        else if (name == "texture_reflection")
            number = std::to_string(reflectionNr++);
        else if (name == "texture_metalness")
            number = std::to_string(metalnessNr++);
        else if (name == "texture_ao")
            number = std::to_string(aoNr++);

        // Set the sampler uniform and bind the texture
        std::string uniformName = "material." + name + number;
        shader.setInt(uniformName.c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Reset active texture
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex), (void *)offsetof(MVertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MVertex), (void *)offsetof(MVertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex), (void *)offsetof(MVertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex), (void *)offsetof(MVertex, biTangent));

    glBindVertexArray(0);
}

void MTexture::loadTexture(const char *path)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char *pixels = stbi_load(path, &width, &height, &channels, 0);
    if (pixels)
    {
        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        throw std::runtime_error("Failed to load texture");

    stbi_image_free(pixels);
}

bool MTexture::loadTexture(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = path;
    filename = directory + '/' + filename;

    glGenTextures(1, &id);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *pixels = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (pixels)
    {
        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        throw std::runtime_error("Failed to load texture");

    stbi_image_free(pixels);
    return true;
}
