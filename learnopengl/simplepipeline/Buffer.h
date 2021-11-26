//
// Created by pan on 2021/11/24.
//

#pragma once

namespace sp{

enum BufferType {
    VAO,
    VBO,
    EBO
};

template <typename T>
class Buffer {

public:

    static Buffer<T>* createVBO(T *data,int size);
    static Buffer<T>* createVAO(T *data,int size);
    static Buffer<T>* createEBO(T *data,int size);

    BufferType bufferType;
    T *data;
    int size;
};



}
