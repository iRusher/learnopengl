//
// Created by pan on 2021/11/24.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Device.h"
#include "Pipeline.h"

namespace sp {


class App {

public:
    App();

    void run();
    void close();

    Pipeline *getDefaultPipeline();

private:

    void init();

private:
    GLFWwindow *window;
    Device *device;

    Pipeline *defaultPipeline;



};
}
