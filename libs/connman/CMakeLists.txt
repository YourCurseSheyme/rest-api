cmake_minimum_required(VERSION 3.31)
project(ConnmanHandler LANGUAGES CXX)

add_library(connman_handler STATIC
        connman_handler.cpp
        connman_handler.hpp
)

set(CMAKE_CXX_STANDARD 17)

target_include_directories(connman_handler PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)