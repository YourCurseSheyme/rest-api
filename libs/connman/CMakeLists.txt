cmake_minimum_required(VERSION 3.31)
project(ConnmanHandler LANGUAGES CXX)

add_library(connman_handler STATIC
#        connman_handler.cpp
#        connman_handler.hpp
        connman_handler.cpp
        connman_handler.hpp
)

target_include_directories(connman_handler PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/libs/utils
)

target_link_libraries(connman_handler PRIVATE utils)