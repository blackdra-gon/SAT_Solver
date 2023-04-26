//
// Created by benjamin on 26.04.23.
//
#include <catch2/catch_test_macros.hpp>

#include "../io_utils.h"

TEST_CASE("Output in dimacs format") {
    std::vector<bool> assignment = {0,1,0,1};
    REQUIRE(output_dimacs({0,1,0,1}) == "v -1 2 -3 4");
}