cmake_minimum_required(VERSION 3.20)
project(programming_basics_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(programming_basics_project
        src/global_var.h
        src/main.cpp
        src/part1.h
        src/part2.h
        src/part3.h
        src/part4.h)
