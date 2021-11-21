cmake_minimum_required(VERSION 3.17)
project(simplerender)

set(CMAKE_CXX_STANDARD 14)

# GLFW
add_subdirectory(glfw)
include_directories(glfw/include)

#GLAD
set(GLAD_SRC glad/src/glad.c)
add_library(glad STATIC ${GLAD_SRC})
include_directories(glad/include)

#GLM
add_subdirectory(glm)
include_directories(glm)

#assimp
add_subdirectory(assimp)
include_directories(assimp/include)

#imgui
add_subdirectory(imgui)

#imgui-docking
add_subdirectory(imgui-docking)

#imgui-gizmo
add_subdirectory(imgui-gizmo)

add_subdirectory(learnopengl)