cmake_minimum_required(VERSION 3.12)
project(LAB6)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR} D:\\boost\\boost-build\\include\\boost-1_84 D:\\boost\\boost-build\\lib) 

add_executable(main main.cpp)
target_compile_options(main PUBLIC -std=c++20 -Wall -Wextra -lboost_regex)
