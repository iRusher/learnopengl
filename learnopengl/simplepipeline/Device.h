//
// Created by pan on 2021/11/24.
//

#pragma once

namespace sp {
struct Device {
    int screenWidth;
    int screenHeight;

    int scale = 1; // Retina = 2

    Device():screenWidth(800),screenHeight(600),scale(2.0){}

    int GetScreenWidth() { return screenWidth * scale; }
    int GetScreenHeight() { return screenHeight * scale; }

};
}




