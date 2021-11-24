configure_file(cube.vs cube.vs COPYONLY)
configure_file(cube.fs cube.fs COPYONLY)

configure_file(frameshader.vs frameshader.vs COPYONLY)
configure_file(frameshader.fs frameshader.fs COPYONLY)

configure_file(light_cube.fs light_cube.fs COPYONLY)
configure_file(light_cube.vs light_cube.vs COPYONLY)

configure_file(container.png container.png COPYONLY)
configure_file(container2_specular.png container2_specular.png COPYONLY)

configure_file(room.obj room.obj COPYONLY)
configure_file(test3.obj test3.obj COPYONLY)
configure_file(test4.obj test4.obj COPYONLY)

add_executable(learn_testcombine main.cpp)
target_link_libraries(learn_testcombine glfw)
target_link_libraries(learn_testcombine glad)
target_link_libraries(learn_testcombine imgui)
target_link_libraries(learn_testcombine learncommon)


#add_executable(learn_stenciltest stenciltest.cpp)
#target_link_libraries(learn_stenciltest glfw)
#target_link_libraries(learn_stenciltest glad)
#target_link_libraries(learn_stenciltest imgui)
#target_link_libraries(learn_stenciltest learncommon)