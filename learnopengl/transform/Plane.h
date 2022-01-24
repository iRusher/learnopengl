//
// Created by Rusher on 2022/1/10.
//


#pragma once

#include <vector>
#include <glad/glad.h>

namespace sr {
    class Plane {
    public:
        Plane() = default;
        ~Plane();

        void init();
        void render();

    private:
        void initBuffer(std::vector<GLfloat> &position,
                        std::vector<GLfloat> &normal,
                        std::vector<GLfloat> &texCoord,
                        std::vector<GLfloat> &tang,
                        std::vector<GLuint> &element);

        std::vector<GLuint> _buffers;

        GLuint _vao{};
        GLuint _vertCount;
        void deleteBuffers();
    };
}// namespace sr
