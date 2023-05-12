//
// Created by benjamin on 26.04.23.
//
#include <catch2/catch_test_macros.hpp>

#include "../io_utils.h"
#include "../dpll.h"
#include "../encoding_util.h"

INITIALIZE_EASYLOGGINGPP

TEST_CASE("Output in dimacs format") {
    std::vector<bool> assignment = {0,1,0,1};
    REQUIRE(output_dimacs({0,1,0,1}) == "v -1 2 -3 4");
}

TEST_CASE("Apply Unit Propagation") {
    Cnf cnf_1 = import_from_file("../../example_dimacs/unit_propagation_test_1.cnf");
    Cnf result = apply_unit_propagation(cnf_1);
    REQUIRE(result.clauses.empty());
    Cnf cnf_2_before = import_from_file("../../example_dimacs/unit_propagation_test_2_before.cnf");
    Cnf cnf_2_after = import_from_file("../../example_dimacs/unit_propagation_test_2_before.cnf");
    Cnf result_2 = apply_unit_propagation(cnf_2_before);
    REQUIRE(cnf_2_after == result_2);
}

TEST_CASE("Negate Literal") {
    REQUIRE(negate_literal(0) == 1);
    REQUIRE(negate_literal(1) == 0);
    REQUIRE(negate_literal(10) == 11);
    REQUIRE(negate_literal(13) == 12);
}