#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "camera.h"
#include "shader_m.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <iostream>
#include "Cube.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void imguiInit(GLFWwindow *window);
void imguiSetup();
void imguiDraw();
void imguiRendering();

glm::mat4 camerViewMat();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 objectColor(1.0f, 0.0f, 0.0f);

bool mousePressed = false;
glm::vec3 cameraUP = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraRotation(0, 45.0f, 0);
glm::vec3 cameraTranslation(0.0f);

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 getModelMatrix() {
        return glm::mat4(1.0f);
    }
};


int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //    glfwSetCursorPosCallback(window, mouse_callback);
    //    glfwSetScrollCallback(window, scroll_callback);
    //    glfwSetMouseButtonCallback(window,mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    imguiInit(window);


    Cube cube;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        imguiSetup();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        imguiDraw();
        imguiRendering();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition.z -= deltaTime * 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition.z += deltaTime * 1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition.x -= deltaTime * 1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition.x += deltaTime * 1;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPosition.y += deltaTime * 1;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPosition.y -= deltaTime * 1;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

glm::vec3 rotateVector(float rx, float ry, glm::vec3 &&v) {
    glm::mat4 rotate(1.0);
    rotate = glm::rotate(rotate, ry, glm::vec3(0.0, 1.0, 0.0));
    rotate = glm::rotate(rotate, rx, glm::vec3(1.0, 0.0, 0.0));
    glm::vec4 result = rotate * glm::vec4(v, 1.0);
    return glm::vec3(result.x, result.y, result.x);
}

void rotate(float xoffset, float yoffset) {
    double rot_x = cameraRotation.x;
    double rot_y = cameraRotation.y;
    double rot_z = cameraRotation.z;

    rot_x = 0.1 * (yoffset);
    rot_y = 0.1 * (-xoffset);
    printf("rot_y %f\n", rot_y);

    cameraRotation.x += rot_x;
    cameraRotation.y += rot_y;

    printf("cameraRotation %f,%f,%f\n", cameraRotation.x, cameraRotation.y, cameraRotation.z);

    glm::mat4 cameraModel(1.0);
    cameraModel = glm::rotate(cameraModel, glm::radians(cameraRotation.z), glm::vec3(0.0, 0.0, 1.0));
    cameraModel = glm::rotate(cameraModel, glm::radians(cameraRotation.x), glm::vec3(1.0, 0.0, 0.0));
    cameraModel = glm::rotate(cameraModel, glm::radians(cameraRotation.y), glm::vec3(0.0, 1.0, 0.0));
    cameraModel = glm::translate(cameraModel, glm::vec3(0, 0, 3));

    glm::vec4 pos(cameraPosition, 1.0);
    glm::vec4 newP = cameraModel * pos;
    printf("cameraPos %f,%f,%f\n", newP.x, newP.y, newP.z);
    cameraPosition = newP.xyz();
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    if (mousePressed) {
        printf("%f %f\n", xoffset, yoffset);
        rotate(xoffset, yoffset);
    }
}

glm::mat4 camerViewMat() {
    glm::mat4 model(1.0);
    model = glm::rotate(model, cameraRotation.z, glm::vec3(0, 0, 1));
    model = glm::rotate(model, cameraRotation.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, cameraRotation.y, glm::vec3(0, 1, 0));
    model = glm::translate(model, cameraTranslation);
    return model;
}


void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_1) {
        mousePressed = (action == GLFW_PRESS);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    //    camera.ProcessMouseScroll(yoffset);
}

void imguiInit(GLFWwindow *window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char *glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
};


void imguiSetup() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imguiDraw() {
    ImGui::Begin("test");
    ImGui::Text("test");
    ImGui::End();
}

void imguiRendering() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
