//
// Created by pan on 2021/11/24.
//

#pragma once

namespace sp {

    class Pipeline;
    class Camera;
    class Shader;

    class RenderPass {

    public:
        RenderPass(){init();};
        void init();
        virtual void beginRenderPass();
        virtual void render(Camera *camera);
        virtual void endRenderPass();

        virtual ~RenderPass();

        void setPipeline(Pipeline *pipeline);

    private:
        Pipeline *_pipeline = nullptr;
        Shader *_shader = nullptr;
    };

}// namespace sp
