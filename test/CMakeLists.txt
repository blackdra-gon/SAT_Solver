find_package(Catch2 3 REQUIRED)
add_executable(tests test.cpp ../io_utils.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)