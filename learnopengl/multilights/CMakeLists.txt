configure_file(cube.vs cube.vs COPYONLY)
configure_file(cube.fs cube.fs COPYONLY)

configure_file(light_cube.fs light_cube.fs COPYONLY)
configure_file(light_cube.vs light_cube.vs COPYONLY)

configure_file(container.png container.png COPYONLY)
configure_file(container2_specular.png container2_specular.png COPYONLY)

include_directories(../include)
add_executable(learn_multilights main.cpp)

target_link_libraries(learn_multilights glfw)
target_link_libraries(learn_multilights glad)
target_link_libraries(learn_multilights imgui)
