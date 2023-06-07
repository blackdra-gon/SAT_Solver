cmake_minimum_required(VERSION 3.25)
project(SAT_Solver)

set(CMAKE_CXX_STANDARD 20)
#include("/home/benjamin/.clion-vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")
include_directories("/home/benjamin/Documents/Programmieren/lorina/include")
#set(CMAKE_FIND_DEBUG_MODE 1)
#find_package(fmt CONFIG REQUIRED)





add_executable(SAT_Solver main.cpp io_utils.cpp dpll.cpp encoding_util.cpp solver_structs.cpp)

add_subdirectory(test)

#target_link_libraries(SAT_Solver PRIVATE fmt::fmt)



