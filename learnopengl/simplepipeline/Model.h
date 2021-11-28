//
// Created by pan on 2021/11/24.
//

#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

namespace sp {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textCoord;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
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
        unsigned int VAO{},VBO{},EBO{};
        void setupMesh();

    };

    class Model {

    public:
        Model(std::string path);
    private:

        void load();
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

        std::vector<Mesh> _meshes;
        std::string _path;
        std::string _directory;

    };

}// namespace sp
