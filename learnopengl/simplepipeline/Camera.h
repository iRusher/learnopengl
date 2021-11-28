//
// Created by pan on 2021/11/24.
//

#pragma once
namespace sp {

    class Scene;

    class Camera {
    public:

        void setScene(Scene *scene);
        Scene *getScene();

    private:
        Scene *_scene;

    };
}// namespace sp