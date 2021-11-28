//
// Created by pan on 2021/11/24.
//

#include "Camera.h"

using namespace sp;

void Camera::setScene(Scene *scene) {
    _scene = scene;
}

Scene *Camera::getScene() {
    return _scene;
}
