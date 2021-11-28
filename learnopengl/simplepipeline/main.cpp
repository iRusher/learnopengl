//
// Created by pan on 2021/11/24.
//

#include "App.h"
#include "memory/AllocObject.h"

#include "Scene.h"

#include <iostream>
#include <vector>

int main() {

    sp::App app;
    sp::Scene *scene = new sp::Scene;
    app.run(scene);

    app.close();
    

    return 0;
}