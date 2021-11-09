

configure_file(1.color.fs 1.color.fs COPYONLY)
configure_file(1.color.vs 1.color.vs COPYONLY)
configure_file(1.light_cube.fs 1.light_cube.fs COPYONLY)
configure_file(1.light_cube.vs 1.light_cube.vs COPYONLY)

include_directories(../include)
add_executable(learn_color main.cpp)

target_link_libraries(learn_color glfw)
target_link_libraries(learn_color glad)
