//
// Created by pan on 2021/11/24.
//

#include "App.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "Device.h"
#include "Scene.h"

using namespace sp;

App::App() {
    init();
}

void App::init() {
    _device = new sp::Device;
    setupWindow();
    setupDefaultPipeline();
}

void App::setupWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "SimplePipeline", NULL, NULL);
    if (window == nullptr) {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwTerminate();
    }

    _window = window;
}

void App::setupDefaultPipeline() {
    _defaultPipeline = new Pipeline;
}

void App::run(Scene *scene) {
    if (_scene) {
        _scene->destroy();
        delete _scene;
    }

    _scene = scene;
    _defaultPipeline->render(_scene->getCameras());

    mainloop();
}


void App::mainloop() {
    while (!glfwWindowShouldClose(_window)) {

        _defaultPipeline->render(_scene->getCameras());

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void App::close() {

    delete _defaultPipeline;
    delete _device;

    _scene = nullptr;

    glfwTerminate();
}


Pipeline *App::getDefaultPipeline() {
    return _defaultPipeline;
}
