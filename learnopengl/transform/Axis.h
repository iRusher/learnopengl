//
// Created by pan on 2022/1/25.
//

#pragma once
#include <iostream>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "glcheck.h"

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};


class Axis {

public:
    Axis() = default;


    void init() {

        Vertex points[] = {
                {{0.0f, 0.0f, 0.0f}, {1.0, 0.0, 0.0, 1.0}},
                {{1.0f, 0.0f, 0.0f}, {1.0, 0.0, 0.0, 1.0}},
                {{0.0f, 1.0f, 0.0f}, {1.0, 0.0, 0.0, 1.0}}};

        GLuint vbo;
        GL_CHECK(glGenBuffers(1, &vbo));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW));

        GL_CHECK(glGenVertexArrays(1, &_vao));
        GL_CHECK(glBindVertexArray(_vao));

        GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr));
        GL_CHECK(glEnableVertexAttribArray(0));

        GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (offsetof(Vertex, color))));
        GL_CHECK(glEnableVertexAttribArray(1));

        GL_CHECK(glBindVertexArray(0));
    }

    void render() {
        GL_CHECK(glBindVertexArray(_vao));
        GL_CHECK(glDrawArrays(GL_LINES, 0, 3));
        GL_CHECK(glBindVertexArray(0));
    };

private:
    GLuint _vao;
};