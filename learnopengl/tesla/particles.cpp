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

#include "glcheck.h"
#include "Log.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Represents a single particle and its state
struct Particle {
    glm::vec3 position, velocity;
    glm::vec4 color;
    GLfloat life;
    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
    Particle(glm::vec3 v, GLfloat l) : position(0.0f), velocity(v), color(1.0f), life(l) {}
};


//class ParticleEmitter {
//
//public:
//    ParticleEmitter() : _amount(500), _particleShader(Shader("particles.vs", "particles.fs")), _texture(Texture("particles.png")) {
//        init();
//    };
//
//    void draw() {
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//        this->_particleShader.use();
//        glBindVertexArray(this->_vao);
//
//        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
//        glm::mat4 view = glm::lookAt(glm::vec3(5, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//        glm::mat4 model(1.0f);
//        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
//
//        this->_particleShader.setMat4("projection", projection);
//        this->_particleShader.setMat4("view", view);
//        this->_particleShader.setMat4("model", model);
//
//        for (Particle particle : this->_particles) {
//            if (particle.life > 0.0f) {
//
//                this->_particleShader.setVec2("offset", particle.position);
//                this->_particleShader.setVec4("color", particle.color);
//                glBindTexture(GL_TEXTURE_2D, _texture.getTextureId());
//                glDrawArrays(GL_TRIANGLES, 0, 6);
//            }
//        }
//        glBindVertexArray(0);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    }
//
//
//    void update(float dt, int newAmount, glm::vec2 position, glm::vec2 velocity) {
//
//        for (int i = 0; i < newAmount; ++i) {
//            int unusedParticles = this->firstUnusedParticle();
//            LOG_DEBUG("%d", unusedParticles);
//            this->respawnParticle(this->_particles[unusedParticles], position, velocity);
//        }
//
//        for (int i = 0; i < this->_amount; ++i) {
//            Particle &p = this->_particles[i];
//            p.life -= dt;
//            if (p.life > 0.0f) {
//                //                LOG_DEBUG("particle %f %f", p.position.x, p.position.y);
//                p.position.x += p.velocity.x * dt;
//                p.color.a -= dt * 2.5;
//            }
//        }
//    }
//
//
//    int firstUnusedParticle() {
//        for (int i = _lastUsedParticle; i < this->_amount; ++i) {
//            LOG_DEBUG("fff %d", i);
//            if (this->_particles[i].life <= 0.0f) {
//                _lastUsedParticle = i;
//                return i;
//            }
//        }
//
//        for (int i = 0; i < _lastUsedParticle; ++i) {
//            if (this->_particles[i].life <= 0.0f) {
//                _lastUsedParticle = i;
//                return i;
//            }
//        }
//
//        _lastUsedParticle = 0;
//        return 0;
//    }
//
//    void respawnParticle(Particle &particle, glm::vec2 &position, glm::vec2 &veclocity) {
//        //        GLfloat random = ((rand() % 100) - 50) / 10.0f;
//        //        GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
//        particle.position = position;
//        //        particle.position = position + random;
//        //        particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
//        particle.color = glm::vec4(1.0, 0.0, 0.0, 1.0);
//        particle.life = 3.0f;
//        particle.velocity = veclocity;
//    }
//
//
//private:
//    void init() {
//        GLuint VBO;
//        GLfloat particle_quad[] = {
//                0.0f, 1.0f, 0.0f, 1.0f,
//                1.0f, 0.0f, 1.0f, 0.0f,
//                0.0f, 0.0f, 0.0f, 0.0f,
//
//                0.0f, 1.0f, 0.0f, 1.0f,
//                1.0f, 1.0f, 1.0f, 1.0f,
//                1.0f, 0.0f, 1.0f, 0.0f};
//
//        GLuint VAO;
//        glGenVertexArrays(1, &VAO);
//        glGenBuffers(1, &VBO);
//        glBindVertexArray(VAO);
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) 0);
//        glBindVertexArray(0);
//
//        this->_vao = VAO;
//        this->_vbo = VBO;
//
//        _particles.reserve(_amount);
//        for (int i = 0; i < _amount; ++i) {
//            _particles.emplace_back(glm::vec2(1, 1), 1);
//        }
//    }
//
//
//    std::vector<Particle> _particles;
//    int _amount;
//
//    Shader _particleShader;
//    Texture _texture;
//
//    GLuint _vao;
//    GLuint _vbo;
//
//    int _lastUsedParticle = 0;
//};


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
        p.velocity = glm::vec3(0, 1,0);

        p.position = glm::vec3(0,0,0);

        p.color = glm::vec4(1.0, 0.0, 0.0, 1.0);
        particles.push_back(p);
    }


    glEnable(GL_DEPTH_TEST);

    int lastParticles = 0;
    int newAmount = 10;

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_BLEND);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(3, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

        box.bindVAO();
        boxShader.use();
        glBindTexture(GL_TEXTURE_2D, container.getTextureId());
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

        // update
        for (int i = lastParticles; (i < lastParticles + newAmount) && (i < particles.size()); ++i) {
            LOG_DEBUG("%d", i);
            Particle &p = particles[i];
            p.life = 5.0f;
        }
        lastParticles += newAmount;

        for (int i = particles.size() - 1; i >= 0; --i) {
            Particle &p = particles[i];
            p.position += deltaTime * p.velocity;
            p.life -= deltaTime;

            if (p.life >= 0.0f) {
                p.color.a = (p.life / 3);
                particleShader.setVec3("offset", p.position);
                particleShader.setVec4("color", p.color);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }


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