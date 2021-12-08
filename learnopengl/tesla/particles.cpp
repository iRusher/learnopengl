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


#include "glcheck.h"
#include "Log.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Represents a single particle and its state
struct Particle {
    glm::vec2 position, velocity;
    glm::vec4 color;
    GLfloat life;
    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};


class ParticleGenerator {

public:
    ParticleGenerator() : _amount(500), _particleShader(Shader("particles.vs", "particles.fs")), _texture(Texture("particles.png")) {
        init();
    };

    void draw() {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        this->_particleShader.use();

        for (Particle particle : this->_particles)
        {
            if (particle.life > 0.0f)
            {
                this->_particleShader.setVec2("offset", particle.position);
                this->_particleShader.setVec4("color", particle.color);
                glBindTexture(GL_TEXTURE_2D,_texture.getTextureId());
                glBindVertexArray(this->_vao);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
            }
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void update(float delta,int newAmount) {

        

    }


private:
    void init() {
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
        // Fill mesh buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
        // Set mesh attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) 0);
        glBindVertexArray(0);

        this->_vao = VAO;
        this->_vbo = VBO;

        _particles.reserve(_amount);
        for (int i = 0; i < _amount; ++i) {
            _particles.emplace_back();
        }

    }


    std::vector<Particle> _particles;
    int _amount;

    Shader _particleShader;
    Texture _texture;


    GLuint _vao;
    GLuint _vbo;
};


int particleAmount = 500;

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
    //    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader particleShader("particles.vs", "particles.fs");
    Texture texture("particles.png");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        particleShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(3, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 model(1.0f);

        particleShader.setMat4("projection", projection);
        particleShader.setMat4("view", view);
        particleShader.setMat4("model", model);


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