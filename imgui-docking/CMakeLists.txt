add_library(imguidocking STATIC)
target_sources(imguidocking
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
        imguidocking
        PUBLIC .
        PUBLIC ./backends
)

add_executable(imguidockingdemo)
target_sources(imguidockingdemo PUBLIC ./examples/example_glfw_opengl3/main.cpp)
target_link_libraries(imguidockingdemo glfw)
target_link_libraries(imguidockingdemo glad)
target_link_libraries(imguidockingdemo imguidocking)