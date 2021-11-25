//
// Created by pan on 2021/11/25.
//

#pragma once

#include <cstdlib>
#include "Allocator.h"

namespace sp {

template<class Allocator>
class AllocObject {

public:
    explicit AllocObject() = default;
    virtual ~AllocObject() = default;

    void *operator new (size_t sz) {
        return sp::Allocator::AllocateBytes(sz);
    }

    void *operator new[](size_t sz) {
        return sp::Allocator::AllocateBytes(sz);
    }

    void operator delete (void *ptr) {
        sp::Allocator::DeallocateBytes(ptr);
    }

    void operator delete[] (void *ptr) {
        sp::Allocator::DeallocateBytes(ptr);
    }

};

}


