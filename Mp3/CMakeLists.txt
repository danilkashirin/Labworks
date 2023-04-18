cmake_minimum_required(VERSION 3.12)

project(
        labwork-7-danilkashirin
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Lab7"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)

include_directories(bin)
include_directories(lib)
include_directories(frames)

add_executable(labwork-7-danilkashirin
        main.cpp
        lib/argument_parser.h lib/argument_parser.cpp
        lib/header.h lib/header.cpp
        frames/frame_base.cpp frames/frame_base.h
        frames/frame_parse.h frames/frame_parse.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/FrameHelp.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)