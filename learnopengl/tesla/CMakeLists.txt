configure_file(cube.fs cube.fs COPYONLY)
configure_file(cube.vs cube.vs COPYONLY)
configure_file(particles.vs particles.vs COPYONLY)
configure_file(particles.fs particles.fs COPYONLY)
configure_file(particles.png particles.png COPYONLY)

include_directories(../include)
add_executable(learn_tesla main.cpp Orbit.cpp)

target_link_libraries(learn_tesla glfw)
target_link_libraries(learn_tesla glad)
target_link_libraries(learn_tesla imgui)
target_link_libraries(learn_tesla imguizmo)


add_executable(learn_spline spline.cpp)

add_executable(learn_particles particles.cpp)
target_link_libraries(learn_particles glfw)
target_link_libraries(learn_particles glad)
target_link_libraries(learn_particles imgui)
target_link_libraries(learn_particles imguizmo)
target_link_libraries(learn_particles learncommon)