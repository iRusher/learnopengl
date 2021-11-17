configure_file(cube.vs cube.vs COPYONLY)
configure_file(cube.fs cube.fs COPYONLY)

configure_file(light_cube.fs light_cube.fs COPYONLY)
configure_file(light_cube.vs light_cube.vs COPYONLY)

configure_file(container.png container.png COPYONLY)
configure_file(container2_specular.png container2_specular.png COPYONLY)

configure_file(cone.obj cone.obj COPYONLY)

#configure_file(earth/untitled.obj earth/untitled.obj COPYONLY)
#configure_file(earth/untitled.mtl earth/untitled.mtl COPYONLY)
#configure_file(earth/earth.png earth/earth.png COPYONLY)

include_directories(../include)
add_executable(learn_lighttool main.cpp)
add_custom_command(
        TARGET learn_lighttool POST_BUILD
        COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/earth ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(learn_lighttool glfw)
target_link_libraries(learn_lighttool glad)
target_link_libraries(learn_lighttool imgui)
target_link_libraries(learn_lighttool learncommon)

add_executable(learn_tooltest toottest.cpp)
target_link_libraries(learn_tooltest glfw)
target_link_libraries(learn_tooltest glad)
target_link_libraries(learn_tooltest imgui)


