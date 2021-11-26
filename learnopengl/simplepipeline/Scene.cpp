//
// Created by pan on 2021/11/24.
//

#include "Scene.h"

#include "Camera.h"
#include "Model.h"

#include <iostream>

namespace sp {

    Scene::Scene() {
        init();
    }

    void Scene::init() {

        Camera *camera = new Camera;
        _cameras.push_back(camera);
    }

    void Scene::addModel(Model *model) {
        if (model)
            _models.emplace_back(model);
    }

    std::vector<Model *> &Scene::getModesl() {
        return _models;
    }

    std::vector<Camera *> &Scene::getCameras() {
        return _cameras;
    }

    Scene::~Scene() {
        std::cout << "in ~Scene()" << std::endl;

        for (Camera *camera : _cameras) {
            delete camera;
        }

        for (Model *model : _models) {
            delete model;
        }
    }


}// namespace sp
