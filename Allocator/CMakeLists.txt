cmake_minimum_required(VERSION 3.12)

project(
        labwork-9-danilkashirin
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Lab9"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)

include_directories(Allocator)
add_executable(labwork-9-danilkashirin
        main.cpp
        Allocator/Allocator.h
        Allocator/Arguments.h
        Allocator/Pool.h Allocator/Sort.h)

