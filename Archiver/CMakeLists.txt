cmake_minimum_required(VERSION 3.12)

project(
        labwork-4-danilkashirin
        VERSION 1.0
        DESCRIPTION "C++ Year 2022 Lab3"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)

include_directories(bin)
include_directories(lib)

add_executable(labwork-4-danilkashirin
        main.cpp
        lib/parser.h lib/parser.cpp
        lib/archiver_voids.h lib/archiver_voids.cpp
        lib/hamming.h lib/hamming.cpp)