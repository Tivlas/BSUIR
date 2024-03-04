cmake_minimum_required(VERSION 3.12)
project(LAB3_DOS)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR} D:\\boost\\boost-build\\include\\boost-1_84 D:\\boost\\boost-build\\lib) 

add_executable(client client.cpp)
target_compile_options(client PUBLIC -std=c++20 -Wall -Wextra)

add_executable(server server.cpp)
target_compile_options(server PUBLIC -std=c++20 -Wall -Wextra)