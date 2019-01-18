cmake_minimum_required(VERSION 3.12)
project(BlockMultiplication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/local/bin/g++-8)
set(CMAKE_CXX_FLAGS "-O3 -fopenmp")

add_executable(BlockMultiplication main.cpp Block.cpp Block.h)
#find_package(OpenMP REQUIRED)
#target_link_libraries(BlockMultiplication PRIVATE OpenMP::OpenMP_CXX)
