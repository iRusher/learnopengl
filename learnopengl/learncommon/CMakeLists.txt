
add_library(learncommon STATIC)
target_sources(learncommon PRIVATE Model.cpp Log.cpp)

target_include_directories(learncommon PUBLIC ../include)
target_link_libraries(learncommon assimp)