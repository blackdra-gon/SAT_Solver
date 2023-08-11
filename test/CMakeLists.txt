set(CMAKE_FIND_DEBUG_MODE 1)
#list(APPEND CMAKE_MODULE_PATH "/home/benjamin/Documents/Programmieren/SAT_Solver/cmake-build-debug/vcpkg_installed/x64-linux/share/catch2/")
find_package(Catch2 CONFIG REQUIRED)
find_package( Boost REQUIRED COMPONENTS serialization )
include_directories( ${Boost_INCLUDE_DIRS} )
add_executable(tests test.cpp ../io_utils.cpp ../encoding_util.cpp ../solver_structs.cpp ../solver.cpp ../clause.cpp ../clause.h test_util.h
        ../clauseRef.cpp ../clauseRef.h ../variable_order.cpp ../variable_order.h)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ${Boost_LIBRARIES})
