//
// Created by Rusher on 2021/11/21.
//

#pragma once

#include <vector>


namespace sp {

    class Camera;

    class Model;
    class Scene;

    class Pipeline {

    public:
        void render(std::vector<Camera *> &cameras);

    private:
        std::shared_ptr<Scene> _scene;
    };

}// namespace sp
