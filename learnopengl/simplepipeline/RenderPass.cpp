//
// Created by pan on 2021/11/24.
//

#include "RenderPass.h"

#include "Camera.h"
#include "Shader.h"
#include "Scene.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Orbit.h"

using namespace sp;

void RenderPass::init() {
    _shader = new Shader("./shaders/cube.vs", "./shaders/cube.fs");
}

void RenderPass::beginRenderPass() {

    _shader->use();
}

void RenderPass::render(Camera *camera) {
    beginRenderPass();

    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    _shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    _shader->setVec3("dirLight.ambient", 1.0f, 1.0f, 1.05f);
    _shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    _shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    _shader->setMat4("projection", projection);
    _shader->setMat4("view", view);
    _shader->setVec3("viewPos", glm::vec3(0, 0, 3));

    glm::mat4 model(1.0);
    model = glm::translate(model,glm::vec3(0,-1,0));
    model = glm::scale(model,glm::vec3(0.1,0.1,0.1));
    _shader->setMat4("model",model);

    auto *scene = camera->getScene();
    std::vector<Model *> &models = scene->getModels();
    for (Model *model : models) {
        model->Draw(*_shader);
    }

    endRenderPass();
}

void RenderPass::endRenderPass() {
}

void RenderPass::setPipeline(Pipeline *pipeline) {
    _pipeline = pipeline;
}

RenderPass::~RenderPass() {
    if (_shader) delete _shader;
}
