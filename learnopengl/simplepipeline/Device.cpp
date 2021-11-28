//
// Created by pan on 2021/11/24.
//

#include "Device.h"

using namespace sp;

static Device retina(800, 600, 2);

Device &Device::getRetinaDevice() {
    return retina;
}