add_library(Geometry Point.h Vector.h Grid.h CartesianGrid.cpp CartesianGrid.h)

target_include_directories(Geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
