cmake_minimum_required(VERSION 3.31)
project(Wps LANGUAGES CXX)

add_library(wps_handler STATIC
        wps.cpp
        wps.hpp
)

target_include_directories(wps_handler PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/libs/utils
)

target_link_libraries(wps_handler PRIVATE utils)