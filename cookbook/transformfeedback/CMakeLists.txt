#file(COPY ${CMAKE_CURRENT_LIST_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

configure_file(container.png container.png COPYONLY)

include_directories(../../learnopengl/include)
add_executable(cookbook_transformfeedback main.cpp RandomTexture.h)

target_link_libraries(cookbook_transformfeedback glfw)
target_link_libraries(cookbook_transformfeedback glad)
target_link_libraries(cookbook_transformfeedback imgui)
target_link_libraries(cookbook_transformfeedback imguizmo)

add_custom_command(
        TARGET cookbook_transformfeedback POST_BUILD
        COMMAND cp -r ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}
)