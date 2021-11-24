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
    BufferType bufferType;
    T *data;
    int size;
};


}
