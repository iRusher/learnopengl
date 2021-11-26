//
// Created by pan on 2021/11/24.
//

#include "Scene.h"

#include "Model.h"
#include "Camera.h"

#include <iostream>

namespace sp {

    Scene::Scene() {
        init();
    }

    void Scene::init() {

        _cameras = new std::vector<Camera*>;
        Camera *camera = new Camera;
        _cameras->push_back(camera);

        _models = new std::vector<Model *>;

        std::cout << "in Scene()" << std::endl;
    }

    void Scene::addModel(Model *model) {
        if (model)
            _models->emplace_back(model);
    }

    std::vector<Model *> *Scene::getModesl() {
        return _models;
    }

    std::vector<Camera *> *Scene::gemCameras() {
        return _cameras;
    }

    Scene::~Scene() {
        std::cout << "in ~Scene()" << std::endl;

        for (Camera *camera:*_cameras) {
            delete camera;
        }
        delete _cameras;

        for (Model *model: *_models) {
            delete model;
        }
        delete _models;
    }


}
