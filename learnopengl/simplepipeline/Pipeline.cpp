//
// Created by Rusher on 2021/11/21.
//

#include "Pipeline.h"

#include "Camera.h"
#include "Scene.h"
#include "RenderPass.h"

#include <iostream>

using namespace sp;

void Pipeline::init() {

    auto *renderPass = new RenderPass;
    passes.push_back(renderPass);
}


void Pipeline::render(std::vector<Camera *> &cameras) {
    for (Camera *camera : cameras) {
        for (auto *pass :passes) {
            pass->render(camera);
        }
    }
}
