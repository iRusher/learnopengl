#include <cstdlib>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader_m.h"
#include "Texture.h"
#include "Cube.h"
#include "camera.h"

#include "glcheck.h"
#include "Log.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// window callbacks
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);

void imguiInit(GLFWwindow *window);
void imguiSetup();

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// Represents a single particle and its state
struct Particle {
    glm::vec3 position, velocity;
    glm::vec4 color;
    GLfloat life;
    GLfloat cameradistance;
    float size, angle, weight;
    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
    Particle(glm::vec3 v, GLfloat l) : position(0.0f), velocity(v), color(1.0f), life(l) {}

    bool operator<(const Particle &that) const {
        return this->cameradistance > that.cameradistance;
    }
};


const int MaxParticles = 500;
Particle particlesContainer[MaxParticles];
int lastUsedParticle = 0;
int findUnusedParticle() {

    for (int i = lastUsedParticle; i < MaxParticles; i++) {
        if (particlesContainer[i].life < 0) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < lastUsedParticle; i++) {
        if (particlesContainer[i].life < 0) {
            lastUsedParticle = i;
            return i;
        }
    }

    return 0;// All particles are taken, override the first one
}

void SortParticles() {
    std::sort(&particlesContainer[0], &particlesContainer[MaxParticles]);
}


float lastFrame;
float deltaTime;

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
    //    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    imguiInit(window);

    Shader particleShader("particles.vs", "particles.fs");
    Texture texture("particles.png");

    //    ParticleEmitter emitter;
    //    emitter.update(deltaTime, 10, glm::vec2(0.0, 0.0), glm::vec2(2.0, 0.0));

    Cube box;
    Texture container("container.png");
    Shader boxShader("box.vs", "box.fs");

    GLuint VBO;
    GLfloat particle_quad[] = {
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f};

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) 0);
    glBindVertexArray(0);

    std::vector<Particle> particles;
    particles.reserve(100);

    for (int i = 0; i < 100; ++i) {
        Particle p;
        p.life = 0.0f;
        p.velocity = glm::vec3(0, -1, 1);

        float x = (rand() % 100 - 50.0f) / 10.0f;
        //        float z =
        p.position = glm::vec3(x, 0, 0);

        p.color = glm::vec4(1.0, 0.0, 0.0, 1.0);
        particles.push_back(p);
    }


    glEnable(GL_DEPTH_TEST);

    int lastParticles = 0;
    int newAmount = 10;

    //range value
    static float f32_right = -5.0f;
    static float f32_left = 5.0f;
    static float f32_up = 5.0f;
    static float f32_down = -5.0f;
    static float f32_zero = 0.0f;
    static float f32_one = 1.0f;
    static float f32_min_g = -9.8f;
    static float f32_max_g = 9.8f;

    // air condition params
    static float f32_horizontal = 0.0f;
    static float f32_vertical = 0.0f;
    static float f32_strength = 0.5f;

    // particle params
    static float f32_gravatiy = 0.5f;
    static float particle_colors[4];

    static bool showDemo;
    float clearColor[4];


    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_BLEND);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1.0f);

        box.bindVAO();
        boxShader.use();
        glBindTexture(GL_TEXTURE_2D, container.getTextureId());
        model = glm::translate(model, glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(4, 1, 1));
        boxShader.setMat4("view", view);
        boxShader.setMat4("model", model);
        boxShader.setMat4("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        particleShader.use();
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

        particleShader.setMat4("projection", projection);
        particleShader.setMat4("view", view);
        particleShader.setMat4("model", model);

        glBindTexture(GL_TEXTURE_2D, texture.getTextureId());
        glBindVertexArray(VAO);

        int newparticles = (int) (deltaTime * 10000.0);
        if (newparticles > (int) (0.016f * 10000.0))
            newparticles = (int) (0.016f * 10000.0);

        for (int i = 0; i < newparticles; i++) {
            int particleIndex = findUnusedParticle();
            particlesContainer[particleIndex].life = 5.0f;// This particle will live 5 seconds.
            particlesContainer[particleIndex].position = glm::vec3(0, 0, -20.0f);

            float spread = 1.5f;
            glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
            glm::vec3 randomdir = glm::vec3(
                    (rand() % 2000 - 1000.0f) / 1000.0f,
                    (rand() % 2000 - 1000.0f) / 1000.0f,
                    (rand() % 2000 - 1000.0f) / 1000.0f);

            particlesContainer[particleIndex].velocity = maindir + randomdir * spread;

            particlesContainer[particleIndex].color.r = rand() % 256;
            particlesContainer[particleIndex].color.g = rand() % 256;
            particlesContainer[particleIndex].color.b = rand() % 256;
            particlesContainer[particleIndex].color.a = (rand() % 256) / 3;

            particlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;
        }

        // Simulate all particles
        int ParticlesCount = 0;
        for (int i = 0; i < MaxParticles; i++) {

            Particle &p = particlesContainer[i];// shortcut

            if (p.life > 0.0f) {

                p.life -= deltaTime;
                if (p.life > 0.0f) {

                    p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float) deltaTime * 0.5f;
                    p.position += p.velocity * (float) deltaTime;
                    p.cameradistance = glm::length(p.position - camera.Position);

                } else {
                    p.cameradistance = -1.0f;
                }
                ParticlesCount++;
            }
        }

        SortParticles();
        for (int i = 0; i < MaxParticles; i++) {
            Particle &p = particlesContainer[i];// shortcut
            if (p.life > 0.0f) {
                particleShader.setVec3("offset", p.position);
                particleShader.setVec4("color", p.color);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        //        // update
        //        for (int i = lastParticles; (i < lastParticles + newAmount) && (i < particles.size()); ++i) {
        //            LOG_DEBUG("%d", i);
        //            Particle &p = particles[i];
        //            p.life = 5.0f;
        //        }
        //        lastParticles += newAmount;
        //
        //        for (int i = particles.size() - 1; i >= 0; --i) {
        //            Particle &p = particles[i];
        //
        //            float t = 5.0 - p.life;
        //            p.position.y = -(3.0f * t * t) / 2.0f;
        //            p.position.z = 3.0 * t;
        //
        //
        //            p.life -= deltaTime;
        //
        //            if (p.life >= 0.0f) {
        //                p.color.a = (p.life / 3);
        //                particleShader.setVec3("offset", p.position);
        //                particleShader.setVec4("color", p.color);
        //                glDrawArrays(GL_TRIANGLES, 0, 6);
        //            }
        //        }

        imguiSetup();

        ImGui::ShowDemoWindow(&showDemo);

        ImGui::Begin("AirCondition");
        ImGui::SliderScalar("Horizontal", ImGuiDataType_Float, &f32_horizontal, &f32_left, &f32_right);
        ImGui::SliderScalar("Vertical", ImGuiDataType_Float, &f32_vertical, &f32_up, &f32_down);
        ImGui::SliderScalar("Strength", ImGuiDataType_Float, &f32_strength, &f32_zero, &f32_one);
        ImGui::End();

        ImGui::Begin("Particles");
        ImGui::ColorEdit4("Clear Color", clearColor);
        ImGui::SliderScalar("Gravity", ImGuiDataType_Float, &f32_gravatiy, &f32_min_g, &f32_max_g);
        ImGui::ColorEdit4("Particle Color", particle_colors);
        ImGui::End();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS)
        firstMouse = true;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS) return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
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