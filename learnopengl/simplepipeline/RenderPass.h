//
// Created by pan on 2021/11/24.
//

#pragma once

namespace sp {

    class Pipeline;

    class RenderPass {

    public:
        virtual void beginRenderPass();
        virtual void render();
        virtual void endRenderPass();

        void setPipeline(Pipeline *pipeline);

    private:
        Pipeline *_pipeline;
    };

}// namespace sp
