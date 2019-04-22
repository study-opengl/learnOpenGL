#ifndef Model_h
#define Model_h

#include <vector>
#include <string>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "ShaderProgram.hpp"
#include "Mesh.hpp"
#include "asLog.h"
#include "openGlHelper.hpp"

unsigned int TextureFromFile(const std::string path, const std::string &directory, bool gamma = false);

class Model
{
  private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> texture_loaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);

  public:
    Model(std::string path);
    void Draw(ShaderProgram shader);
};

Model::Model(std::string path)
{
    loadModel(path);
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        asLog("ERROR::ASSIMP::%s\n", import.GetErrorString());
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::Draw(ShaderProgram shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> verties;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        aiVector3D aiPosition = mesh->mVertices[i];
        vertex.Position.x = aiPosition.x;
        vertex.Position.y = aiPosition.y;
        vertex.Position.z = aiPosition.z;

        aiVector3D aiNormal = mesh->mNormals[i];
        vertex.Normal.x = aiNormal.x;
        vertex.Normal.y = aiNormal.y;
        vertex.Normal.z = aiNormal.z;

        if (mesh->mTextureCoords[0])
        {
            aiVector3D aiTexCoords = mesh->mTextureCoords[0][i];
            vertex.TexCoords.x = aiTexCoords.x;
            vertex.TexCoords.y = aiTexCoords.y;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0, 0.0);
        }
        verties.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    
    return Mesh(verties, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    unsigned int textureCount = material->GetTextureCount(type);
    for (unsigned int i = 0; i < textureCount; i++)
    {
        aiString path;
        material->GetTexture(type, i, &path);
        bool skip = false;
        for (unsigned int j = 0; j < texture_loaded.size(); j++)
        {
            if (std::strcmp(texture_loaded[j].path.c_str(), path.C_Str()) == 0)
            {
                textures.push_back(texture_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(std::string(path.C_Str()), directory);
            texture.type = typeName;
            texture.path = std::string(path.C_Str());
            textures.push_back(texture);
            texture_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const std::string path, const std::string &directory, bool gamma)
{
    std::string filename = directory + "/" + path;

    unsigned int textureID = textureGenarate(filename.c_str());
    return textureID;
}

#endif
