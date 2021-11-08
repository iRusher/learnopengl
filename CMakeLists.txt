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

add_subdirectory(learnopengl)

#target_link_libraries(simplerender glfw)
#target_link_libraries(simplerender glad)
