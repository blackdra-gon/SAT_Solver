cmake_minimum_required(VERSION 3.25)
project(SAT_Solver)

set(CMAKE_CXX_STANDARD 20)

find_package( Boost REQUIRED COMPONENTS serialization )
include_directories( ${Boost_INCLUDE_DIRS} )

add_executable(SAT_Solver main.cpp io_utils.cpp encoding_util.cpp solver_structs.cpp solver.cpp clause.cpp clause.h clauseRef.cpp clauseRef.h variable_order.cpp variable_order.h)

target_link_libraries( SAT_Solver ${Boost_LIBRARIES} )

add_subdirectory(test)
add_subdirectory(sat_checker)




