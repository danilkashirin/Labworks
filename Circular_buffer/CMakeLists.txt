cmake_minimum_required(VERSION 3.12)

project(
        labwork-8-danilkashirin
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Lab8"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)

add_executable(labwork-8-danilkashirin
        main.cpp
        CCircularBuffer.h
        CCircularBufferExt.h
        Iterator/Iterator.h Iterator/ConstIterator.h)

add_subdirectory(tests)
enable_testing()