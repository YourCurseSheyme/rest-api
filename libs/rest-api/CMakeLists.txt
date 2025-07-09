cmake_minimum_required(VERSION 3.31)
project(RestAPI LANGUAGES CXX)

find_package(oatpp 1.4.0 REQUIRED)

add_library(rest_api STATIC
        rest_handler.cpp
        rest_handler.hpp
        components.hpp
        controller/controller.hpp
        controller/controller.cpp
        dto/dtos.hpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_compile_options(rest_api PRIVATE "-std=c++17")

target_link_libraries(rest_api PUBLIC
        oatpp::oatpp
        connman_handler
)

target_include_directories(rest_api PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/libs/connman/
)