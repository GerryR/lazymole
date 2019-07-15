include_directories(${CMAKE_SOURCE_DIR}/Geometry ${Boost_INCLUDE_DIRS})

add_library(Fields Field.h CellField.h)

target_include_directories(Fields PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
