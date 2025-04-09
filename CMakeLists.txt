cmake_minimum_required(VERSION 3.5)
project(3d-renderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)

add_executable(test test.cpp)
target_link_libraries(test PUBLIC 3d_renderer)
