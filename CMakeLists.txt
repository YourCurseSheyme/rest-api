cmake_minimum_required(VERSION 3.31)
project(rest-api LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs)

add_executable(rest-api libs/main.cpp)

target_link_libraries(rest-api PRIVATE
        connman_handler
        wps_handler
        rest_api
)