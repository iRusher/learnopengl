configure_file(cube.vs cube.vs COPYONLY)
configure_file(cube.fs cube.fs COPYONLY)

configure_file(light_cube.fs light_cube.fs COPYONLY)
configure_file(light_cube.vs light_cube.vs COPYONLY)

configure_file(container.png container.png COPYONLY)
configure_file(container2_specular.png container2_specular.png COPYONLY)

configure_file(quad.vs quad.vs COPYONLY)
configure_file(quad.fs quad.fs COPYONLY)

add_executable(learn_framebuffer main.cpp)

target_link_libraries(learn_framebuffer glfw)
target_link_libraries(learn_framebuffer glad)
target_link_libraries(learn_framebuffer imgui)
target_link_libraries(learn_framebuffer learncommon)

add_custom_command(
        TARGET learn_framebuffer POST_BUILD
        COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/../resources/nanosuit ${CMAKE_CURRENT_BINARY_DIR}
)