//
// Created by pan on 2021/11/24.
//

#include "Scene.h"

#include "Camera.h"
#include "Model.h"

#include <iostream>

using namespace sp;

Scene::Scene() {
    init();
}

void Scene::init() {

    Camera *camera = new Camera;
    camera->setScene(this);
    _cameras.push_back(camera);

    Model *model = new Model("./nanosuit/nanosuit.obj");
    _models.push_back(model);
}

void Scene::addModel(Model *model) {
    if (model)
        _models.emplace_back(model);
}

std::vector<Model *> &Scene::getModels() {
    return _models;
}

std::vector<Camera *> &Scene::getCameras() {
    return _cameras;
}

Scene::~Scene() {

}

void Scene::destroy() {
    for (Camera *camera : _cameras) {
        delete camera;
    }

    for (Model *model : _models) {
        delete model;
    }
}
