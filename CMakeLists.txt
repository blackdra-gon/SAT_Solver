cmake_minimum_required(VERSION 3.25)
project(SAT_Solver)

set(CMAKE_CXX_STANDARD 17)
include_directories("/home/benjamin/Documents/Programmieren/lorina/include")


add_compile_options("-lfmt")
add_executable(SAT_Solver main.cpp)



