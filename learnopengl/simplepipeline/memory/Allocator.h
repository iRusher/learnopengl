//
// Created by pan on 2021/11/25.
//

#pragma once

#include <cstdlib>
#include <memory>

namespace sp {
class Allocator {

public:

    static void *AllocateBytes(size_t size) {
        void *ptr = malloc(size);
        return ptr;
    }

    static void *ReallocateBytes(void *ptr,size_t size) {
        return realloc(ptr,size);
    }

    static void DeallocateBytes(void *ptr) {
        free(ptr);
    }

private:
    Allocator(){}

};

}



