//
// Created by pan on 2021/11/24.
//

#include "Buffer.h"
#include <glad/glad.h>

namespace sp {

template<typename T>

Buffer<T> *Buffer<T>::createVBO(T *data, int size) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    return nullptr;
}

template<typename T>
Buffer<T> *Buffer<T>::createVAO(T *data, int size) {


    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * size, data, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // textcoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    return nullptr;
}

template<typename T>
Buffer<T> *Buffer<T>::createEBO(T *data, int size) {
    return nullptr;
}


}