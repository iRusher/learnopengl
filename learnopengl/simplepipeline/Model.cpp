//
// Created by pan on 2021/11/24.
//

#include "Model.h"

#include <glad/glad.h>
#include <iostream>
#include <unordered_map>

#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace sp;

Model::Model(std::string path):_path(std::move(path)) {
    load();
}

void Model::load() {

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(_path,aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    }
    _directory = _path.substr(0,_path.find_last_of('/'));
    processNode(scene->mRootNode,scene);

}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh,scene));
    }

    for (int i = 0; i < node->mNumChildren; ++i) {
        aiNode *childNode = node->mChildren[i];
        processNode(childNode,scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        vertex.position = position;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        vertex.normal = normal;

        glm::vec2 textCoord(1.0f);
        if (mesh->mTextureCoords[0]) {
            textCoord.x = mesh->mTextureCoords[0][i].x;
            textCoord.y = mesh->mTextureCoords[0][i].y;
        }
        vertex.textCoord = textCoord;

        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
        textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR,"texture_specular");
        textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());
    }

    return Mesh(vertices,indices,textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type,i,&str);

        std::string filename(directory);
        filename = filename + "/" + str.C_Str();
        if (textCache.find(filename) != textCache.end()) {
            textures.push_back(textCache.find(filename)->second);
        } else {
            Texture texture;
            texture.id = loadTextureFromFile(filename);
            texture.type = typeName;
            textures.push_back(texture);
            texture.path = filename;

            textCache[filename] = texture;
        }
    }
    return textures;
}

