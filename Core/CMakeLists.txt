include_directories(${CMAKE_SOURCE_DIR}/Geometry ${CMAKE_SOURCE_DIR}/Fields ${Boost_INCLUDE_DIRS})

add_library(Core LazyMole.h)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
