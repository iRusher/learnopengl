
add_library(learncommon STATIC)
target_sources(learncommon PRIVATE Model.cpp)

target_include_directories(learncommon PUBLIC ../include)