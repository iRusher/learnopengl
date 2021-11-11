configure_file(cube.vs cube.vs COPYONLY)
configure_file(directional.fs directional.fs COPYONLY)
configure_file(point.fs point.fs COPYONLY)
configure_file(spot.fs spot.fs COPYONLY)

configure_file(light_cube.fs light_cube.fs COPYONLY)
configure_file(light_cube.vs light_cube.vs COPYONLY)

configure_file(container.png container.png COPYONLY)
configure_file(container2_specular.png container2_specular.png COPYONLY)

include_directories(../include)
add_executable(learn_lightcasters main.cpp)

target_link_libraries(learn_lightcasters glfw)
target_link_libraries(learn_lightcasters glad)
target_link_libraries(learn_lightcasters imgui)
