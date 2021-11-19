configure_file(1.color.fs 1.color.fs COPYONLY)
configure_file(1.color.vs 1.color.vs COPYONLY)
configure_file(container.png container.png COPYONLY)
configure_file(container.jpg container.jpg COPYONLY)
configure_file(awesomeface.png awesomeface.png COPYONLY)
configure_file(window.png window.png COPYONLY)

configure_file(quad.fs quad.fs COPYONLY)
configure_file(quad.vs quad.vs COPYONLY)

include_directories(../include)
add_executable(learn_texture_fbo main.cpp)

target_link_libraries(learn_texture_fbo glfw)
target_link_libraries(learn_texture_fbo glad)
