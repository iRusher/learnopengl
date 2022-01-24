//
// Created by Rusher on 2022/1/10.
//

#include "Plane.h"

#include <vector>

#include <glad/glad.h>

#define GL_CHECK(x) x;

namespace sr {

    void Plane::init() {

        float xSize = 0.4;
        float zSize = 3.0;

        int xDiv = 2;
        int zDiv = 30;

        // uv
        float sMax = 1.0f;
        float tMax = 1.0f;

        int pointCnt = (xDiv + 1) * (zDiv + 1);

        std::vector<GLfloat> position(pointCnt * 3); // 顶点
        std::vector<GLfloat> normal(pointCnt * 3);   // 法线
        std::vector<GLfloat> texCoord(pointCnt * 2); // 纹理坐标
        std::vector<GLfloat> tang(pointCnt * 3);     // 切线空间
        std::vector<GLuint> element(xDiv * zDiv * 6);// EBO

        // 让平面的中点在原点
        float x2 = xSize / 2.0f;
        //        float z2 = zSize / 2.0f;

        float xFactor = (float) xSize / xDiv;
        float zFactor = (float) zSize / zDiv;
        float texX = sMax / xDiv;
        float texZ = tMax / zDiv;

        float x, z;
        int vIdx = 0, tIdx = 0;

        for (int i = 0; i <= zDiv; ++i) {
            z = zFactor * i;
            for (int j = 0; j <= xDiv; ++j) {
                x = xFactor * j - x2;

                position[vIdx] = x;
                position[vIdx + 1] = 0.0f;
                position[vIdx + 2] = z;

                normal[vIdx] = 0.0f;
                normal[vIdx + 1] = 1.0f;
                normal[vIdx + 2] = 0.0f;

                texCoord[tIdx] = j * texX;
                texCoord[tIdx + 1] = (zDiv - i) * texZ;

                //                LOG_DEBUG("%f %f %f,%f %f", x, 0.0, z, j * texX, (zDiv - i) * texZ);

                vIdx += 3;
                tIdx += 2;
            }
        }

        for (int i = 0; i < pointCnt; ++i) {
            tang[i * 4 + 0] = 1.0f;
            tang[i * 4 + 1] = 0.0f;
            tang[i * 4 + 2] = 0.0f;
            tang[i * 4 + 3] = 1.0f;
        }

        GLuint rowStart, nextRowStart;
        int idx = 0;
        for (int i = 0; i < zDiv; i++) {
            rowStart = (GLuint) (i * (xDiv + 1));
            nextRowStart = (GLuint) ((i + 1) * (xDiv + 1));
            for (int j = 0; j < xDiv; j++) {

                element[idx + 0] = rowStart + j;
                element[idx + 1] = nextRowStart + j;
                element[idx + 2] = nextRowStart + j + 1;
                element[idx + 3] = rowStart + j;
                element[idx + 4] = nextRowStart + j + 1;
                element[idx + 5] = rowStart + j + 1;

                idx += 6;
            }
        }

        initBuffer(position, normal, texCoord, tang, element);
    }

    void Plane::initBuffer(
            std::vector<GLfloat> &position,
            std::vector<GLfloat> &normal,
            std::vector<GLfloat> &texCoord,
            std::vector<GLfloat> &tang,
            std::vector<GLuint> &element) {

        _vertCount = element.size();

        GLuint indexBuf;
        GL_CHECK(glGenBuffers(1, &indexBuf));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(GLuint), element.data(), GL_STATIC_DRAW));

        GLuint posBuf;
        GL_CHECK(glGenBuffers(1, &posBuf));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, posBuf));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(GLfloat), position.data(), GL_STATIC_DRAW));

        GLuint normalBuf;
        GL_CHECK(glGenBuffers(1, &normalBuf));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, normalBuf));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW));

        GLuint texBuf;
        GL_CHECK(glGenBuffers(1, &texBuf));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, texBuf));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(GLfloat), texCoord.data(), GL_STATIC_DRAW));

        // tang may be later
        _buffers.push_back(indexBuf);
        _buffers.push_back(posBuf);
        _buffers.push_back(normalBuf);
        _buffers.push_back(texBuf);

        GL_CHECK(glGenVertexArrays(1, &_vao));
        GL_CHECK(glBindVertexArray(_vao));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf));

        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, posBuf));
        GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
        GL_CHECK(glEnableVertexAttribArray(0));

        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, normalBuf));
        GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
        GL_CHECK(glEnableVertexAttribArray(1));

        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, texBuf));
        GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr));
        GL_CHECK(glEnableVertexAttribArray(2));

        GL_CHECK(glBindVertexArray(0));
    };

    Plane::~Plane() {
        deleteBuffers();
    }

    void Plane::render() {
        if (_vao != 0) {
            GL_CHECK(glBindVertexArray(_vao));
            GL_CHECK(glDrawElements(GL_TRIANGLES, _vertCount, GL_UNSIGNED_INT, nullptr));
            GL_CHECK(glBindVertexArray(0));
        }
    }

    void Plane::deleteBuffers() {
        if (!_buffers.empty()) {
            glDeleteBuffers(_buffers.size(), _buffers.data());
        }

        if (_vao != 0) {
            glDeleteVertexArrays(1, &_vao);
            _vao = 0;
        }
    }

}// namespace sr