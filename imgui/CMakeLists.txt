add_library(imgui STATIC)
target_sources(imgui
PRIVATE
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

target_include_directories(
        imgui
        PUBLIC .
        PUBLIC ./backends
)

add_executable(imguidemo)
target_sources(imguidemo PUBLIC ./examples/example_glfw_opengl3/main.cpp)
target_link_libraries(imguidemo glfw)
target_link_libraries(imguidemo glad)
target_link_libraries(imguidemo imgui)