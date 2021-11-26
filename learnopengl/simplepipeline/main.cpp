//
// Created by pan on 2021/11/24.
//

#include "App.h"
#include "memory/AllocObject.h"

#include "Scene.h"

#include <iostream>

int main() {

    sp::App app;
    std::shared_ptr<sp::Scene> scene(new sp::Scene());
    app.run(scene);

    app.close();

    return 0;
}