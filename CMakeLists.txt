cmake_minimum_required(VERSION 3.20)
project(programming_basics_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(programming_basics_project
        main.cpp
        part3.h part1.h part2.h part4.h global_var.h)
