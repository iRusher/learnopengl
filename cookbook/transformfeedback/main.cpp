#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader_m.h"
#include "Texture.h"

#include <iostream>
#include "glcheck.h"
#include "Plane.h"
#include "RandomTexture.h"


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
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //    Plane plane;
    //    plane.bindVAO();
    // VAO
    // VBO1
    // VBO1 data
    // VBO1 vao pointer
    // VBO1 vao divisor

    GLuint vao;
    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glBindVertexArray(vao));

    std::vector<GLfloat> vertices;
    int verticesCount = 10;
    for (int i = 0; i < 10; ++i) {
        vertices.push_back(i * 0.1);
        vertices.push_back(0);
        vertices.push_back(0);
    }
    GLuint vbo;
    GL_CHECK(glGenBuffers(1, &vbo));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));
    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr));
    GL_CHECK(glEnableVertexAttribArray(0));

    std::vector<GLfloat> deltaData;
    for (int i = 0; i < verticesCount; ++i) {
        deltaData.push_back(0.1 * i);
    }
    GLuint deltaVBO;
    GL_CHECK(glGenBuffers(1, &deltaVBO));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, deltaVBO));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, deltaData.size() * sizeof(GLfloat), deltaData.data(), GL_STATIC_DRAW));

    GL_CHECK(glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), nullptr));
    GL_CHECK(glEnableVertexAttribArray(1));
    //    GL_CHECK(glVertexAttribDivisor(1, 1));

    GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GLuint feedbackDataBuffer;
    GL_CHECK(glGenBuffers(1, &feedbackDataBuffer));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, feedbackDataBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), nullptr, GL_DYNAMIC_COPY));

    GLuint feedbackBuffer;
    GL_CHECK(glGenTransformFeedbacks(1, &feedbackBuffer));
    GL_CHECK(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbackBuffer));
    GL_CHECK(glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedbackDataBuffer));
    GL_CHECK(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GLuint feedbackVAO;
    GL_CHECK(glGenVertexArrays(1, &feedbackVAO));
    GL_CHECK(glBindVertexArray(feedbackVAO));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, feedbackDataBuffer));
    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    const char *outputNames[] = {"outPos"};
    Shader cubeShader("shaders/cube.vs", "shaders/cube.fs", nullptr, outputNames);
    cubeShader.use();

    GLint tLoc = glGetUniformLocation(cubeShader.ID, "t");
    GLint containerLoc = glGetUniformLocation(cubeShader.ID, "container");
    glUniform1i(tLoc, 0);
    glUniform1i(containerLoc, 1);

    GLuint textureId = RandomTexture::create1D(10);
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_1D, textureId));

    Texture container("container.png");
    GLuint tid = container.getTextureId();
    GL_CHECK(glActiveTexture(GL_TEXTURE1));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, tid));

    GL_CHECK(glEnable(GL_PROGRAM_POINT_SIZE));
    GL_CHECK(glPointSize(10.0f));

    while (!glfwWindowShouldClose(window)) {

        GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        cubeShader.setInt("pass", 0);
        GL_CHECK(glEnable(GL_RASTERIZER_DISCARD));
        GL_CHECK(glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbackBuffer));
        GL_CHECK(glBeginTransformFeedback(GL_POINTS));
        GL_CHECK(glBindVertexArray(vao));
        GL_CHECK(glDrawArrays(GL_POINTS, 0, verticesCount));
        GL_CHECK(glEndTransformFeedback());
        GL_CHECK(glDisable(GL_RASTERIZER_DISCARD));

        GL_CHECK(glBindVertexArray(feedbackVAO));
        cubeShader.setInt("pass", 1);
        GL_CHECK(glDrawArraysInstanced(GL_POINTS, 0, 10, 10));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
}