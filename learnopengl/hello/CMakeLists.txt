
add_executable(learn_hello main.cpp)

target_link_libraries(learn_hello glfw)
target_link_libraries(learn_hello glad)
target_link_libraries(learn_hello assimp)
target_link_libraries(learn_hello imgui)
