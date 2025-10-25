cmake_minimum_required(VERSION 3.10)

project(distfs)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -pedantic -Werror")

# only for better code
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -Weverything -Werror \
    -Wno-disabled-macro-expansion  \
    -Wno-c23-extensions  \
    -Wno-declaration-after-statement \
    -Wno-cast-align \
    -Wno-unsafe-buffer-usage \
    -Wno-reserved-identifier")

enable_testing()
add_subdirectory(lib)
add_subdirectory(app)
