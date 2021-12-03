//
// Created by pan on 2021/12/3.
//

#pragma once

#include <glad/glad.h>

namespace sp {

    class Cube {
    public:
        Cube() { setupBuffers(); }

        unsigned int getVA0() { return _vao; }
        unsigned int getVBO() { return _vbo; }
        void bindVAO() { glBindVertexArray(_vao); }
        int verticesCount() { return 36; };

    private:
        unsigned int _vao;
        unsigned int _vbo;

        static const float vertices[];

        void setupBuffers();
    };

}// namespace sp
