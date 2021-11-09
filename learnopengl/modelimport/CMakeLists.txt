
configure_file(diablo3_pose.obj diablo3_pose.obj COPYONLY)

add_executable(learn_modelimport main.cpp)

target_link_libraries(learn_modelimport glfw)
target_link_libraries(learn_modelimport glad)
target_link_libraries(learn_modelimport assimp)
