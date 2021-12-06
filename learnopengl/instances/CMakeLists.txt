configure_file(cube.fs cube.fs COPYONLY)
configure_file(cube.vs cube.vs COPYONLY)

include_directories(../include)
add_executable(learn_instances main.cpp)

target_link_libraries(learn_instances glfw)
target_link_libraries(learn_instances glad)
target_link_libraries(learn_instances imgui)
target_link_libraries(learn_instances imguizmo)
target_link_libraries(learn_instances learncommon)
