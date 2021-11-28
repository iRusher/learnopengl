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
        PRIVATE ../include
)

target_link_libraries(simplepipeline glad)
target_link_libraries(simplepipeline glm)
target_link_libraries(simplepipeline glfw)
target_link_libraries(simplepipeline imgui)
target_link_libraries(simplepipeline assimp)


# /////////////////////////////////////////////////
# sprun
add_executable(sprun main.cpp)
target_link_libraries(sprun simplepipeline)
add_custom_command(
        TARGET sprun PRE_BUILD
        COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/../resources/nanosuit ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/../resources/skybox ${CMAKE_CURRENT_BINARY_DIR}
)
