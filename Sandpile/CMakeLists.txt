cmake_minimum_required(VERSION 3.12)

project(
        labwork-3-danilkashirin
        VERSION 1.0
        DESCRIPTION "C++ Year 2022 Lab3"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)

include_directories(bin)
include_directories(lib)

add_executable(labwork-3-danilkashirin
        lib/image.cpp lib/image.h
        lib/parser.cpp lib/parser.h
        sand_pile.cpp)
