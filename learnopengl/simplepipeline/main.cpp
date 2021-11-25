//
// Created by pan on 2021/11/24.
//

#include "App.h"
#include "memory/AllocObject.h"

using Object = sp::AllocObject<sp::Allocator>;

class Test: public Object {
public:
    Test() = default;
};

int main() {

    sp::App app;
    app.run();

    Test *t = new Test();
    delete t;

    return 0;
}