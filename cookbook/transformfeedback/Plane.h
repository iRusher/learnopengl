//
// Created by Rusher on 2021/12/19.
//


#pragma once

#include <vector>
#include <glad/glad.h>

class Plane {
public:
    Plane() { gen(); };
    void bindVAO() const {
        glBindVertexArray(_vao);
    }

    int getVerticesCount() { return _verticesCount; };

private:
    void gen() {

        for (int i = 0; i < _verticesCount; ++i) {
            _vertices.push_back(i * 0.1);
            _vertices.push_back(0);
            _vertices.push_back(0);
        }

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    float width = 5;
    float height = 5;
    float xDiv = 10;
    float yDiv = 10;

    int _verticesCount = 10;
    std::vector<GLfloat> _vertices;

    GLuint _vao;
};
