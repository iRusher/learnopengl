//
// Created by pan on 2021/11/24.
//

#include "RenderPass.h"

#include "Camera.h"
#include "Shader.h"

using namespace sp;

void RenderPass::init() {
    _shader = new Shader("cube.vs","cube.fs");
}


void RenderPass::beginRenderPass() {
}

void RenderPass::render(Camera *camera) {
    beginRenderPass();

    endRenderPass();
}

void RenderPass::endRenderPass() {
}

void RenderPass::setPipeline(Pipeline *pipeline) {
    _pipeline = pipeline;
}

RenderPass::~RenderPass() {
    if (_shader) delete _shader;
}
