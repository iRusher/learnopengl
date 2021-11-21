add_library(imguizmo STATIC)

target_sources(imguizmo
        PRIVATE
        ImGuizmo.cpp
        )
target_include_directories(
        imguizmo
        PUBLIC .
)
target_link_libraries(imguizmo imgui)