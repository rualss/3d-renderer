cmake_minimum_required(VERSION 3.13)
project(3d-renderer)

set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(test test.cpp)
target_link_libraries(test PUBLIC 3d_renderer)
