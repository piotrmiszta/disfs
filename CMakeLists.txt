cmake_minimum_required(VERSION 3.2)

project(distfs)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

enable_testing()
add_subdirectory(lib)
add_subdirectory(app)
