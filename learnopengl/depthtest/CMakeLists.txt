configure_file(cube.vs cube.vs COPYONLY)
configure_file(cube.fs cube.fs COPYONLY)

configure_file(frameshader.vs frameshader.vs COPYONLY)
configure_file(frameshader.fs frameshader.fs COPYONLY)

configure_file(light_cube.fs light_cube.fs COPYONLY)
configure_file(light_cube.vs light_cube.vs COPYONLY)

configure_file(container.png container.png COPYONLY)
configure_file(container2_specular.png container2_specular.png COPYONLY)

configure_file(diablo3_pose.obj diablo3_pose.obj COPYONLY)

add_executable(learn_depthtest main.cpp)
target_link_libraries(learn_depthtest glfw)
target_link_libraries(learn_depthtest glad)
target_link_libraries(learn_depthtest imgui)
target_link_libraries(learn_depthtest learncommon)

add_executable(learn_stenciltest stenciltest.cpp)
target_link_libraries(learn_stenciltest glfw)
target_link_libraries(learn_stenciltest glad)
target_link_libraries(learn_stenciltest imgui)
target_link_libraries(learn_stenciltest learncommon)