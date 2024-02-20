cmake_minimum_required(VERSION 3.12)
project(LAB2)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include_directories(${CMAKE_SOURCE_DIR}) 

add_executable(C C/main.cpp)
target_compile_options(C PUBLIC -std=c++20 -Wall -Wextra)

add_executable(SS SS/main.cpp)
target_compile_options(SS PUBLIC -std=c++20 -Wall -Wextra)

add_executable(KDC KDC/main.cpp)
target_compile_options(KDC PUBLIC -std=c++20 -Wall -Wextra)
