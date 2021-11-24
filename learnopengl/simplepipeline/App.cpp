//
// Created by pan on 2021/11/24.
//

#include "App.h"

#include <GLFW/glfw3.h>

#include "Device.h"
namespace sp {

void App::init() {

    this->device = new sp::Device;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(this->device->GetScreenWidth(), this->device->GetScreenHeight(), "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
    }

    this->window = window;

}

void App::run() {
    while (!glfwWindowShouldClose(window))
    {

    }
    this->close();
}

void App::close() {

}

App::App() {
    this->init();
}

}
