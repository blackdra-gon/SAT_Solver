cmake_minimum_required(VERSION 3.25)
project(SAT_Solver)

set(CMAKE_CXX_STANDARD 17)
include_directories("/home/benjamin/Documents/Programmieren/lorina/include")
find_package(cxxopts CONFIG REQUIRED)




add_compile_options("-lfmt")
add_executable(SAT_Solver main.cpp io_utils.cpp easylogging++.cc dpll.cpp)

add_subdirectory(test)

target_link_libraries(SAT_Solver PRIVATE cxxopts::cxxopts)



