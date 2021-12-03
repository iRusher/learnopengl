configure_file(color.fs color.fs COPYONLY)
configure_file(color.vs color.vs COPYONLY)
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
