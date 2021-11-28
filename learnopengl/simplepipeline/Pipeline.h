//
// Created by Rusher on 2021/11/21.
//

#pragma once

#include <vector>


namespace sp {

    class Camera;

    class Model;
    class Scene;
    class RenderPass;

    class Pipeline {

    public:
        Pipeline() { init(); };
        void init();
        void render(std::vector<Camera *> &cameras);

    private:
        std::vector<RenderPass *> passes;
    };

}// namespace sp
