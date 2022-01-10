//
// Created by pan on 2022/1/4.
//

#pragma once

#include <glad/glad.h>

#include <random>
#include <algorithm>

#include <vector>

class Random {
private:
    std::mt19937 generator;
    std::uniform_real_distribution<float> distr01;

public:
    Random() : distr01(0.0, 1.0) {
        std::random_device rd;
        generator.seed(rd());
    }

    float nextFloat() {
        return distr01(generator);
    }
};


class RandomTexture {
public:
    static GLuint create1D(int size) {
        Random random;

        std::vector<GLfloat> randData(size);
        for (int i = 0; i < size; ++i) {
            randData[i] = random.nextFloat();
            std::cout << randData[i] << std::endl;
        }

        GLuint textureId;
        GL_CHECK(glGenTextures(1, &textureId));
        GL_CHECK(glBindTexture(GL_TEXTURE_1D, textureId));
        GL_CHECK(glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size));
        GL_CHECK(glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data()));

        GL_CHECK(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(glBindTexture(GL_TEXTURE_1D, 0));

        return textureId;
    }

    static glm::mat3 makeArbitraryBasis(const glm::vec3 &dir) {
        glm::mat3 basis;
        glm::vec3 u, v, n;
        v = dir;
        n = glm::cross(glm::vec3(1, 0, 0), v);
        if (glm::length(n) < 0.00001f) {
            n = glm::cross(glm::vec3(0, 1, 0), v);
        }
        u = glm::cross(v, n);
        basis[0] = glm::normalize(u);
        basis[1] = glm::normalize(v);
        basis[2] = glm::normalize(n);
        return basis;
    }
};
