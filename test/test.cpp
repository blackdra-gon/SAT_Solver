//
// Created by benjamin on 26.04.23.
//


#include "../io_utils.h"
#include "test_util.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Input") {
    Solver s;
    import_from_file("../../example_dimacs/unit_test_import.cnf", s);
    std::vector<std::vector<int>> expected_clauses = {{-2, 4, 5}, {-1, 6, -7},
                                                      {-1, -3, -5}, {4, 6, 8},
                                                      {1, 5, -7}};
    check_clauses(s, expected_clauses);
    //s.antecedent_clauses.emplace_back(s.clauses[0]);
    CHECK(s.watch_lists.size() == 20);
    CHECK(s.assignments.size() == 10);
    CHECK(s.watch_lists[internal_representation(-7)].empty());
    CHECK(s.watch_lists[internal_representation(7)].empty());
    CHECK(s.watch_lists[internal_representation(6)].empty());
    CHECK(s.watch_lists[internal_representation(-6)].size() == 2);
    CHECK(s.watch_lists[internal_representation(2)].front().lock() == s.clauses[0]);
}

TEST_CASE("Solver propagation routine") {
    Solver s;
    s.setNumberOfVariables(3);
    s.addClauses({{1,-2,3}, {-1,-3,-2}, {1}});
    CHECK(s.propagate() == std::nullopt);
    check_assignments(s, {TRUE, UNASSIGNED, UNASSIGNED});
    check_clauses(s, {{1,-2,3}, {-3,-2,-1}});
    check_watchlists(s, {{}, {0}, {0,1}, {}, {1}, {}});
    check_trail(s, {1});
    Solver s1;
    s1.setNumberOfVariables(3);
    s1.addClauses({{1,-2,3}, {-1,-3,-2}, {1}, {-1,2}});
    CHECK(s1.propagate() == std::nullopt);
    check_assignments(s1, {TRUE, TRUE, FALSE});
    check_clauses(s1, {{1,-2,3}, {-3,-2,-1}, {2,-1}});
    check_watchlists(s1, {{2}, {0}, {0,1}, {2}, {1}, {}});
    check_trail(s1, {1,2,-3});
}

TEST_CASE("Propagation in one clause") {
    Solver s;
    s.setNumberOfVariables(3);
    s.addClause(internal_representation({1, -2, 3}), false);
    check_watchlists(s, {{},{0},{0},{},{},{}});
    s.clauses[0]->propagate(s, internal_representation(2));
    s.assignments[1] = TRUE;
    // this has to be checked for the general propagation function
    // CHECK(s.watch_lists[internal_representation(2)].empty());
    CHECK(s.watch_lists[internal_representation(-3)].front().lock() == s.clauses[0]);
    CHECK(s.watch_lists[internal_representation(-1)].front().lock() == s.clauses[0]);
    s.clauses[0]->propagate(s, internal_representation(-1));
    CHECK(s.watch_lists[internal_representation(-3)].front().lock() == s.clauses[0]);
    CHECK(s.watch_lists[internal_representation(-1)].front().lock() == s.clauses[0]);
    CHECK(s.propagation_queue.front() == internal_representation(3));
}

TEST_CASE("Unit propagation conflict") {
    Solver s;
    s.setNumberOfVariables(2);
    s.addClauses({{1,-2}, {1,2}, {-1}});
    check_watchlists(s, {{}, {0,1}, {0}, {1}});
    //CHECK(s.propagate() != std::nullopt);
    CHECK(s.propagate() == s.clauses[1]);
    check_watchlists(s, {{}, {0,1}, {0}, {1}});
}

TEST_CASE("Clause Learning") {
    // Example from Handbook of Satisfiability (new Version)
    Solver s;
    s.setNumberOfVariables(8);
    s.addClauses({{-1,-2,3}, {-1,4}, {-3,-4,5}, {-8,-5,6}, {-5,7}, {-6,-7}});
    s.assume(internal_representation(8));
    CHECK(s.propagate() == std::nullopt);
    s.assume(internal_representation(2));
    CHECK(s.propagate() == std::nullopt);
    s.assume(internal_representation(1));
    auto conflicting_clause = s.propagate();
    CHECK(conflicting_clause == s.clauses[5]);
    CHECK(s.current_decision_level() == 3);
    check_decision_levels(s, {3,2,3,3,3,3,3,1});
    check_antecedent_clauses(s, {-1, -1, 0, 1, 2, 3, 4, -1});
    std::vector<Literal_t> learnt_clause;
    int backtrack_level = 0;
    s.analyse_conflict(conflicting_clause.value(), learnt_clause, backtrack_level);
    std::vector<Literal_t> expected_learnt_clause = {internal_representation(-5), internal_representation(-8)};
    CHECK(learnt_clause == expected_learnt_clause);
    CHECK(backtrack_level == 1);

}

TEST_CASE("Clause Learning - minimal") {
    // Simple example: Assignment of one variable leads to contradiction
    Solver s;
    s.setNumberOfVariables(2);
    s.addClauses({{2, -1}, {1, 2}});
    s.assume(internal_representation(-2));
    auto conflicting_clause = s.propagate();
    CHECK(conflicting_clause == s.clauses[1]);
    std::vector<Literal_t> learnt_clause;
    int backtrack_level = 0;
    auto conflict = conflicting_clause.value();
    s.analyse_conflict(conflict, learnt_clause, backtrack_level);
    std::vector<Literal_t> expected_learnt_clause = {internal_representation(2)};
    CHECK(expected_learnt_clause == learnt_clause);
}

TEST_CASE("Clause Learning - 2") {
    // Simple example: Assignment of one variable leads to contradiction
    Solver s;
    s.setNumberOfVariables(3);
    s.addClauses({{1, -3}, {-3, 2}, {-2,-3,-1}});
    s.assume(internal_representation(3));
    auto conflicting_clause = s.propagate();
    CHECK(conflicting_clause == s.clauses[2]);
    std::vector<Literal_t> learnt_clause;
    int backtrack_level = 0;
    auto conflict = conflicting_clause.value();
    s.analyse_conflict(conflict, learnt_clause, backtrack_level);
    std::vector<Literal_t> expected_learnt_clause = {internal_representation(-3)};
    CHECK(expected_learnt_clause == learnt_clause);
}

TEST_CASE("Clause Learning - Varisat Example") {
    // Example from Varisat blog
    Solver s;
    s.setNumberOfVariables(10);
    s.addClauses({{1,2,3,4}, {1,2,3,-4}, {5,6}, {7,8}, {9,10}});
    check_watchlists(s, {{}, {0,1}, {}, {0,1}, {}, {},
                                    /*4*/ {}, {}, {}, {2}, {}, {2},
                                    /*7*/ {}, {3}, {}, {3}, {}, {4}, {}, {4}});
    s.assume(internal_representation(-9));
    CHECK(s.propagate() == std::nullopt);
    s.assume(internal_representation(-2));
    CHECK(s.propagate() == std::nullopt);
    s.assume(internal_representation(-7));
    CHECK(s.propagate() == std::nullopt);
    s.assume(internal_representation(-3));
    CHECK(s.propagate() == std::nullopt);
    s.assume(internal_representation(-6));
    CHECK(s.propagate() == std::nullopt);
    s.assume(internal_representation(-1));
    auto conflicting_clause = s.propagate();
    check_assignments(s, {FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE});
    check_trail(s, {-9, 10, -2, -7, 8, -3, -6, 5, -1, 4});
    check_watchlists(s, {{}, {0,1}, {}, {}, {}, {},
            /*4*/ {1}, {0}, {}, {2}, {}, {2},
            /*7*/ {}, {3}, {}, {3}, {}, {4}, {}, {4}});
    CHECK(conflicting_clause == s.clauses[1]);
    std::vector<Literal_t> learnt_clause;
    int backtrack_level = 0;
    auto conflict = conflicting_clause.value();
    s.analyse_conflict(conflict, learnt_clause, backtrack_level);
    std::vector<Literal_t> expected_learnt_clause = {internal_representation(1), internal_representation(2),
                                                     internal_representation(3)};
    CHECK(expected_learnt_clause == learnt_clause);
    CHECK(backtrack_level == 4);
    s.backtrack_until(backtrack_level);
    s.record_learnt_clause(learnt_clause);
    check_assignments(s, {TRUE, FALSE, FALSE, UNASSIGNED, UNASSIGNED, UNASSIGNED, FALSE, TRUE, FALSE, TRUE});
    check_trail(s, {-9, 10, -2, -7, 8, -3, 1});
    check_watchlists(s, {{}, {0,1,-1}, {}, {}, {}, {-1},
            /*4*/ {1}, {0}, {}, {2}, {}, {2},
            /*7*/ {}, {3}, {}, {3}, {}, {4}, {}, {4}});
}



TEST_CASE("Input/Output in dimacs format") {
    std::vector<bool> assignment = {0,1,0,1};
    REQUIRE(output_dimacs({0,1,0,1}) == "v -1 2 -3 4");
    REQUIRE(internal_representation(-1) == 1);
    REQUIRE(internal_representation(1) == 0);
    REQUIRE(internal_representation(-5) == 9);
    REQUIRE(internal_representation(13) == 24);
    REQUIRE(dimacs_format(1) == -1);
    REQUIRE(dimacs_format(12) == 7);
    REQUIRE(dimacs_format(0) == 1);
}

TEST_CASE("Simple test cases for overall solving routine") {
    Solver s;
    s.setNumberOfVariables(3);
    s.addClauses({{1, -3}, {-3, 2}, {-2,-3,-1}});
    CHECK(s.solve() == true);
    Solver s1;
    s1.setNumberOfVariables(3);
    s1.addClauses({{1, -3}, {-3, 2}, {-2,-3,-1}, {3}});
    CHECK(s1.solve() == false);
    Solver s2;
    s2.setNumberOfVariables(1);
    s2.addClauses({{1}, {-1}});
    // Solver does not yet recognize unit conflicts in given clauses
    //CHECK(s2.solve() == false);

}


/*TEST_CASE("Apply Unit Propagation") {
    Cnf cnf_1 = import_from_file("../../example_dimacs/unit_propagation_test_1.cnf");
    apply_unit_propagation(cnf_1);
    CHECK(cnf_1.clauses.empty());
    Assignments expected_assignments = from_dimacs_list( {-1, -2, -3});
    CHECK(cnf_1.assignments == expected_assignments);
    Cnf cnf_2_before = import_from_file("../../example_dimacs/unit_propagation_test_2_before.cnf");
    Cnf cnf_2_after = import_from_file("../../example_dimacs/unit_propagation_test_2_after.cnf");
    apply_unit_propagation(cnf_2_before);
    CHECK(cnf_2_after == cnf_2_before);
    Assignments expected_assignments_2 = from_dimacs_list( {1});
    CHECK(cnf_2_before.assignments == expected_assignments_2);
    // Unittest for detecting conflicts
    Cnf conflict = import_from_file("../../example_dimacs/unit_propagation_test_simple_conflict.cnf");
    CHECK(!apply_unit_propagation(conflict));
    // function applied to empty clause list
    Cnf cnf_empty;
    CHECK(apply_unit_propagation(cnf_empty));
}*/

TEST_CASE("Negate Literal") {
    REQUIRE(negate_literal(0) == 1);
    REQUIRE(negate_literal(1) == 0);
    REQUIRE(negate_literal(10) == 11);
    REQUIRE(negate_literal(13) == 12);
}

TEST_CASE("Search routine") {
    Solver s;
    s.setNumberOfVariables(10);
    s.addClauses({{-1,-2,-3,-4}, {-1,-2,-3,4}, {5,6}, {7,8}, {9,10}});
    CHECK(s.search() == TRUE);

}
