//
// Created by Rusher on 2021/11/21.
//

#include "Pipeline.h"
#include "Camera.h"
#include "Scene.h"

#include <iostream>

namespace sp {

    void Pipeline::render(std::vector<Camera *> &cameras) {

        for (Camera *camera : cameras) {
        }
    }

    std::vector<Model *> &Pipeline::getRenderObjects() {
    }

    void Pipeline::render(std::shared_ptr<Scene> scene) {
        _scene = scene;
        render(_scene->getCameras());
    }

}// namespace sp
