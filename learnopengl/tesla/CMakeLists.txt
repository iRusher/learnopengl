configure_file(cube.fs cube.fs COPYONLY)
configure_file(cube.vs cube.vs COPYONLY)

include_directories(../include)
add_executable(learn_tesla main.cpp Orbit.cpp)

target_link_libraries(learn_tesla glfw)
target_link_libraries(learn_tesla glad)
target_link_libraries(learn_tesla imgui)
target_link_libraries(learn_tesla imguizmo)


add_executable(learn_spline spline.cpp)