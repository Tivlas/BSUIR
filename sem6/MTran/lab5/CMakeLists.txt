cmake_minimum_required(VERSION 3.12)
project(LAB5)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR} D:\\University\\BSUIR\\sem6\\MTran\\lab3 D:\\University\\BSUIR\\sem6\\MTran\\lab2) 

add_executable(main main.cpp)
target_compile_options(main PUBLIC -std=c++20  -O3)