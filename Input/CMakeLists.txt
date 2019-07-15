include_directories(${YAMLCPP_INCLUDE_DIR})

add_library(Input Input.cpp Input.h)

target_include_directories(Input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} yaml-cpp)