#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "camera.h"
#include "shader_m.h"
#include "Texture.h"

#include <iostream>
#include "Cube.h"
#include "glcheck.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader redShader("red.vs", "red.fs");
    Shader yellowShader("yellow.vs", "yellow.fs");
    Shader blueShader("blue.vs", "blue.fs");
    Shader greenShader("green.vs", "green.fs");

    Cube cube;
    Texture texture("./container.png");

    unsigned int ubo;
    glGenBuffers(1, &ubo);
    GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
    GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW));
    GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    GL_CHECK(glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4)));

    unsigned int redUniformIdx = glGetUniformBlockIndex(redShader.ID, "Matrices");
    unsigned int blueUniformIdx = glGetUniformBlockIndex(blueShader.ID, "Matrices");
    unsigned int yellowUniformIdx = glGetUniformBlockIndex(yellowShader.ID, "Matrices");
    unsigned int greenUniformIdx = glGetUniformBlockIndex(greenShader.ID, "Matrices");

    GL_CHECK(glUniformBlockBinding(redShader.ID, redUniformIdx, 0));
    GL_CHECK(glUniformBlockBinding(blueShader.ID, blueUniformIdx, 0));
    GL_CHECK(glUniformBlockBinding(yellowShader.ID, yellowUniformIdx, 0));
    GL_CHECK(glUniformBlockBinding(greenShader.ID, greenUniformIdx, 0));

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.bindVAO();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
        GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view))); // 这里的顺序和vs中的顺序要一样
        GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection)));

        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
        redShader.use();
        redShader.setMat4("model", model);

        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, cube.verticesCount()));

        blueShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
        redShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, cube.verticesCount());

        yellowShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
        redShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, cube.verticesCount());

        greenShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
        redShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, cube.verticesCount());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
