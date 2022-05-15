cmake_minimum_required(VERSION 3.22)
project(shm_array)

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Python)
