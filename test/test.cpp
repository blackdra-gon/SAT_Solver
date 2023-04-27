//
// Created by benjamin on 26.04.23.
//
#include <catch2/catch_test_macros.hpp>

#include "../io_utils.h"
#include "../dpll.h"

INITIALIZE_EASYLOGGINGPP

TEST_CASE("Output in dimacs format") {
    std::vector<bool> assignment = {0,1,0,1};
    REQUIRE(output_dimacs({0,1,0,1}) == "v -1 2 -3 4");
}

TEST_CASE("Apply Unit Propagation") {
    Cnf cnf = import_from_file("../../example_dimacs/unit_propagation_test_1.cnf");
    Cnf result = apply_unit_propagation(cnf);
    REQUIRE(result.clauses.empty());
}