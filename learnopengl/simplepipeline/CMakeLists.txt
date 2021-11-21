add_library(simplepipeline STATIC)

target_sources(simplepipeline

        PRIVATE Pipeline.cpp
        )

target_include_directories(
        simplepipeline
        PUBLIC .
)

target_link_libraries(simplepipeline glad)
target_link_libraries(simplepipeline glm)
