//
// Created by pan on 2021/11/24.
//

#include "Model.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <unordered_map>

using namespace sp;

void Mesh::setupMesh() {
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned  int),&indices[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void *)0);

//    offsetof(s, m)，它的第一个参数是一个结构体，第二个参数是这个结构体中变量的名字。
//    这个宏会返回那个变量距结构体头部的字节偏移量(Byte Offset)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void *)offsetof(Vertex,normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void *)offsetof(Vertex,textCoord));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {
    for (int i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}


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
        _meshes.push_back(processMesh(mesh,scene));
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

std::unordered_map<std::string,Texture> textCache;
unsigned int loadTextureFromFile(std::string fileName);

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type,i,&str);

        std::string filename(_directory);
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

unsigned int loadTextureFromFile(std::string fileName) {

//    std::cout << fileName << std::endl;

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width,height,nrChannels;

    unsigned char * data = stbi_load(fileName.c_str(),&width,&height,&nrChannels,0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_set_flip_vertically_on_load(true);
    if (data)
    {
        GLint format = GL_RGB;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<< "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture1;
}
