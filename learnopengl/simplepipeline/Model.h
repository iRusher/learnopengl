//
// Created by pan on 2021/11/24.
//

#pragma once

#include <string>

class aiNode;
class aiScene;

namespace sp {

    class Model {

    public:
        Model(std::string path);
    private:

        void load();
        void processNode(aiNode *node, const aiScene *scene);

        std::string _path;
        std::string _directory;
    };
}// namespace sp
