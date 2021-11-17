configure_file(cube.vs cube.vs COPYONLY)
configure_file(cube.fs cube.fs COPYONLY)

configure_file(window.fs window.fs COPYONLY)
configure_file(window.vs window.vs COPYONLY)

configure_file(container.png container.png COPYONLY)
configure_file(container2_specular.png container2_specular.png COPYONLY)
configure_file(window.png window.png COPYONLY)

add_executable(learn_facecull main.cpp)
target_link_libraries(learn_facecull glfw)
target_link_libraries(learn_facecull glad)
target_link_libraries(learn_facecull imgui)
target_link_libraries(learn_facecull learncommon)
