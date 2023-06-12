cmake_minimum_required(VERSION 3.12)

project(
        labwork-12-danilkashirin
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Lab12"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)

include_directories(lib)

add_executable(labwork-12-danilkashirin main.cpp
        lib/Table.h lib/Column.h lib/SqlRequest.h lib/MyCoolDB.h)

add_subdirectory(tests)
enable_testing()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/my_sql
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
