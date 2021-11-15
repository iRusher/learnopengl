//
// Created by pan on 2021/11/15.
//

#include "Model.h"
#include "shader_m.h"
#include <glad/glad.h>


void Mesh::setupMesh() {

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned  int ),&indices[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void *)0);

//    offsetof(s, m)，它的第一个参数是一个结构体，第二个参数是这个结构体中变量的名字。
//    这个宏会返回那个变量距结构体头部的字节偏移量(Byte Offset)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void *)offsetof(Vertex,normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void *) offsetof(Vertex,textCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {

}

void Model::loadModel() {

}

void Model::processNode(aiNode *node, const aiScene *scene) {

}

void Model::processMesh(aiMesh *mesh, const aiScene *scene) {

}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    return std::vector<Texture>();
}