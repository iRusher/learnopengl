//
// Created by Rusher on 2021/12/4.
//


#pragma once

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class Texture {

public:
    Texture(std::string &&path):_path(std::move(path)){ loadTexture();};
    unsigned int getTextureId(){return _textureId;};

private:

    void loadTexture() {
        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width,height,nrChannels;
        unsigned char * data = stbi_load(_path.c_str() ,&width,&height,&nrChannels,0);
        if (!data) {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_set_flip_vertically_on_load(true);
        if (data)
        {
            GLint format = GL_RGB;
            if (nrChannels == 1) {
                format = GL_RED;
            } else if (nrChannels == 3) {
                format = GL_RGB;
            } else if (nrChannels == 4) {
                format = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
    unsigned int _textureId;
    std::string _path;

};
