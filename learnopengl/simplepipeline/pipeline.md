# 对渲染流程的简单封装

教程中的例子基本以下面的流程执行，先看看一次完整的渲染都做了哪些工作，然后再想办法进行抽象。

```cpp

// 窗口初始化，设置默认的Framebuffer
glfwWinodowSetup(); 

modelVerticesInit(); // 顶点数据
bufferInit();  // VAO，VBO，EBO创建
shaderInit();  // Shader加载
textureInit(); // 纹理加载
framebufferInit(); // Framebuffer创建

while(!exitFlag) {
    
    while(passCount > 0) {
        storeGLState(); // 存储GL状态，以便Render Pass结束之后恢复，不影响其他RenderPass
        setupGLState(); // 设置GL状态
        switchShader(); // 切换Shader
        setupUniform(); // 设置ShaderUniform值
        switchFramebuffer(); // 切换Framebuffer
        activeTexture(); // 激活纹理 
        updateMatrix(); // 更新MVP
        drawCall(); // 调用glDrawXXX
        restoreGLState(); // 恢复存储的GL状态
    }
    
    swapFramebuffer();
}

```

初步设计为：

```cpp

App app;
Device device;



```

