configure_file(cube.vs cube.vs COPYONLY)
configure_file(cube.fs cube.fs COPYONLY)

configure_file(skybox.vs skybox.vs COPYONLY)
configure_file(skybox.fs skybox.fs COPYONLY)

configure_file(light_cube.fs light_cube.fs COPYONLY)
configure_file(light_cube.vs light_cube.vs COPYONLY)

configure_file(container.png container.png COPYONLY)
configure_file(container2_specular.png container2_specular.png COPYONLY)

configure_file(quad.vs quad.vs COPYONLY)
configure_file(quad.fs quad.fs COPYONLY)

add_executable(learn_cubemap main.cpp)

target_link_libraries(learn_cubemap glfw)
target_link_libraries(learn_cubemap glad)
target_link_libraries(learn_cubemap imgui)
target_link_libraries(learn_cubemap learncommon)

add_custom_command(
        TARGET learn_cubemap POST_BUILD
        COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/../resources/nanosuit ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/../resources/skybox ${CMAKE_CURRENT_BINARY_DIR}
)
