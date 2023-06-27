cmake_minimum_required(VERSION 3.25)
project(SAT_Solver)

set(CMAKE_CXX_STANDARD 20)

add_executable(SAT_Solver main.cpp io_utils.cpp dpll.cpp encoding_util.cpp solver_structs.cpp solver.cpp clause.cpp clause.h clauseRef.cpp clauseRef.h)

add_subdirectory(test)




