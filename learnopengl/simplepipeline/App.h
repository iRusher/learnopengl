//
// Created by pan on 2021/11/24.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Device.h"
#include "Pipeline.h"

namespace sp {

    class Scene;

    class App {

    public:
        App();
        void run(Scene *scene);
        void close();
        Pipeline *getDefaultPipeline();

    private:
        void init();
        void setupWindow();
        void setupDefaultPipeline();
        void mainloop();

    private:
        GLFWwindow *_window = nullptr;
        Device *_device = nullptr;
        Pipeline *_defaultPipeline = nullptr;
        Scene *_scene = nullptr;
    };
}// namespace sp
