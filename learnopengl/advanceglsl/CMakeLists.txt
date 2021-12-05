configure_file(red.fs red.fs COPYONLY)
configure_file(red.vs red.vs COPYONLY)

configure_file(blue.fs blue.fs COPYONLY)
configure_file(blue.vs blue.vs COPYONLY)

configure_file(green.fs green.fs COPYONLY)
configure_file(green.vs green.vs COPYONLY)

configure_file(yellow.fs yellow.fs COPYONLY)
configure_file(yellow.vs yellow.vs COPYONLY)


configure_file(container.png container.png COPYONLY)
configure_file(container.jpg container.jpg COPYONLY)
configure_file(awesomeface.png awesomeface.png COPYONLY)
configure_file(window.png window.png COPYONLY)

include_directories(../include)
add_executable(learn_advanceglsl main.cpp)

target_link_libraries(learn_advanceglsl glfw)
target_link_libraries(learn_advanceglsl glad)
target_link_libraries(learn_advanceglsl imgui)
target_link_libraries(learn_advanceglsl imguizmo)
