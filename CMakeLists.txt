cmake_minimum_required(VERSION 3.25)
project(SAT_Solver)

set(CMAKE_CXX_STANDARD 20)

add_executable(SAT_Solver main.cpp io_utils.cpp encoding_util.cpp solver_structs.cpp solver.cpp clause.cpp clause.h
        Solver_Stats.cpp
        Solver_Stats.h)

option(CLION_BUILD "Enables Test" OFF)
option(COLLECT_STATS "Collect and output solver statistics" OFF)
option(VERBOSE_LOGGING "Logs the search process (very expensive)" OFF)

#set(INPROCESSING_LOOKBACK_N_CLAUSES 6)

if(CLION_BUILD)
    add_subdirectory(test)
    add_subdirectory(sat_checker)
endif ()

if (COLLECT_STATS)
    add_compile_definitions(-DCOLLECT_SOLVER_STATISTICS=1)
endif ()

if (VERBOSE_LOGGING)
    add_compile_definitions(-DLOG_SEARCH=1)
endif ()

add_definitions(-DLOOKBACK_N_CLAUSES=${INPROCESSING_LOOKBACK_N_CLAUSES})





