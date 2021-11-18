//
// Created by pan on 2021/11/18.
//

#ifndef SIMPLERENDER_GLCHECK_H
#define SIMPLERENDER_GLCHECK_H


#define GL_CHECK(x)                                                         \
    do {                                                                    \
        x; GLenum err = glGetError();                                       \
        if (err != GL_NO_ERROR) {                                           \
            std::cout << #x << "returned GL error " << err << std::endl;    \
        }                                                                   \
    } while (0)

#endif //SIMPLERENDER_GLCHECK_H
