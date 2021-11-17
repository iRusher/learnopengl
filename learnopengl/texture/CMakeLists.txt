configure_file(1.color.fs 1.color.fs COPYONLY)
configure_file(1.color.vs 1.color.vs COPYONLY)
configure_file(container.png container.png COPYONLY)
configure_file(container.jpg container.jpg COPYONLY)
configure_file(awesomeface.png awesomeface.png COPYONLY)
configure_file(window.png window.png COPYONLY)

include_directories(../include)
add_executable(learn_texture main.cpp)

target_link_libraries(learn_texture glfw)
target_link_libraries(learn_texture glad)
