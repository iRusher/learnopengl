configure_file(cube_vs.glsl cube_vs.glsl COPYONLY)
configure_file(cube_fs.glsl cube_fs.glsl COPYONLY)
configure_file(axis_vs.glsl axis_vs.glsl COPYONLY)
configure_file(axis_fs.glsl axis_fs.glsl COPYONLY)

include_directories(../include)
add_executable(learn_transform main.cpp Plane.cpp Plane.h)

target_link_libraries(learn_transform glfw)
target_link_libraries(learn_transform glad)
target_link_libraries(learn_transform imgui)
target_link_libraries(learn_transform imguizmo)
