cmake_minimum_required(VERSION 3.3)
project(LMA)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules
                      ${CMAKE_MODULE_PATH})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
if(MSVC)
    set(BOOST_ROOT C:/Users/GerryR/Documents/Applications/boost_1_55_0/boost_1_55_0)
endif()
find_package(Boost REQUIRED)

set(YAMLCPP_USE_STATIC_LIBS ON)
if(MSVC)
    set(YAML_ROOT C:/Users/GerryR/Documents/YamlCPP/yaml-cpp-win32)
endif()
find_package(YamlCpp REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(MSVC)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            set(${flag_var} "${${flag_var}}" CACHE INTERNAL "")
        endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
endif()

add_subdirectory("Geometry")
add_subdirectory("Fields")
add_subdirectory("Core")
add_subdirectory("Input")

set(SOURCE_FILES main.cpp)
include_directories(${Boost_INCLUDE_DIRS} ${YAMLCPP_INCLUDE_DIR})
add_executable(lazyMole ${SOURCE_FILES})
target_link_libraries(lazyMole LINK_PUBLIC Geometry Fields Core Input ${Boost_LIBRARIES} ${YAMLCPP_LIBRARY})
