//
// Created by pan on 2021/11/24.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Device.h"

namespace sp {


class App {

public:
    App();

    void run();
    void close();

private:
    GLFWwindow *window;
    Device *device;
    void init();
};
}
