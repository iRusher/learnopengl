//
// Created by pan on 2021/11/24.
//

#pragma once

#include <vector>

namespace sp {

    class Model;
    class Camera;
    class Scene {

    public:
        Scene();
        ~Scene();

        void init();
        void addModel(Model *model);
        std::vector<Model *> &getModels();
        std::vector<Camera *> &getCameras();

    private:
        std::vector<Model *> _models{};
        std::vector<Camera *> _cameras{};
    };

}// namespace sp
