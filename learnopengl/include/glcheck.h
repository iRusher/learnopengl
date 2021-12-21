//
// Created by pan on 2021/11/18.
//

#pragma once

#define GL_CHECK(x)                                                                                              \
    do {                                                                                                         \
        x;                                                                                                       \
        GLenum err = glGetError();                                                                               \
        if (err != GL_NO_ERROR) {                                                                                \
            std::cout << __FILE__ << ":" << __LINE__ << " " << #x << " returned GL error. " << err << std::endl; \
        }                                                                                                        \
    } while (0)
