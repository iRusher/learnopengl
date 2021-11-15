//
// Created by pan on 2021/11/15.
//

#ifndef SIMPLERENDER_MODEL_H
#define SIMPLERENDER_MODEL_H

#include <string>
#include <utility>
#include <vector>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader_m.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 textCoords;
};

struct Texture {
    unsigned int id;
    std::string *type;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> v,std::vector<unsigned int> i,std::vector<Texture> t):vertices(std::move(v)),indices(std::move(i)),textures(std::move(t)){
        setupMesh();
    };

    void Draw(Shader &shader);
private:
    unsigned int VAO,VBO,EBO;
    void setupMesh();

};

class Model {
public:
    Model(std::string& path){}

private:
    std::vector<Mesh> meshes;
    void loadModel();
    void processNode(aiNode *node,const aiScene *scene);
    void processMesh(aiMesh *mesh,const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,aiTextureType type,std::string typeName);
};


#endif //SIMPLERENDER_MODEL_H
