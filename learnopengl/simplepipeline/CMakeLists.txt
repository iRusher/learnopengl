add_library(simplepipeline STATIC)

target_sources(simplepipeline
        PRIVATE
        App.cpp
        Buffer.cpp
        Camera.cpp
        Device.cpp
        Framebuffer.cpp
        Light.cpp
        Model.cpp
        Pipeline.cpp
        RenderPass.cpp
        RenderWindow.cpp
        Scene.cpp
        Texture.cpp

        memory/AllocObject.cpp
        )

target_include_directories(
        simplepipeline
        PUBLIC .
)
target_link_libraries(simplepipeline glad)
target_link_libraries(simplepipeline glm)
target_link_libraries(simplepipeline glfw)
target_link_libraries(simplepipeline imgui)

add_executable(sprun main.cpp)
target_link_libraries(sprun simplepipeline)
