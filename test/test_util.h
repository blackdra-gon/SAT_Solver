#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "../solver.h"

//
// Created by benjamin on 21.06.23.
//
void check_assignments(Solver &s, std::vector<lbool> expected_values) {
    CHECK(s.assignments == expected_values);
}

// Takes the indeces of the clause in the watchlist as parameter
// Ideas for making this check weaker: do not check the order inside a watchlist; just check, if the expected clauses
// are a subset of the clause actually in the watchlist
void check_watchlists(Solver &s, const std::vector<std::vector<int>>& expected_clauses) {
    if (s.watch_lists.size() != expected_clauses.size()) {
        std::cout << "Expected and actual watchlists differ in length";
        CHECK(false);
    }
    for (int i = 0; i < s.watch_lists.size(); i++) {
        if (s.watch_lists[i].size() != expected_clauses[i].size()) {
            std::cout << "Expected and actual watchlist differ in length";
            CHECK(false);
        }
        for (int j = 0; j < s.watch_lists[i].size(); ++j) {
            CHECK(s.watch_lists[i][j] == s.clauses[expected_clauses[i][j]]);
        }
    }
}

void check_clauses(Solver &s, const std::vector<std::vector<int>>& expected_clauses) {
    CHECK(s.clauses == internal_representation(expected_clauses));
}

void check_trail(Solver &s, std::vector<int> expected_assignments) {
    std::vector<Literal_t> expected_trail;
    expected_trail.reserve(expected_assignments.size());
for (auto x: expected_assignments) {
        expected_trail.push_back(internal_representation(x));
    }
    CHECK(s.trail == expected_trail);
}

void check_decision_levels(Solver &s, std::vector<int> expected_levels) {
    CHECK(s.decision_levels == expected_levels);
}

void check_antecedent_clauses(Solver &s, std::vector<int> expected_reasons) {
    CHECK(expected_reasons.size() == s.antecedent_clauses.size());
    for (int i = 0; i < expected_reasons.size(); ++i) {
        if (expected_reasons[i] == -1) {
            CHECK(s.antecedent_clauses[i] == std::nullopt);
        } else {
            CHECK(s.antecedent_clauses[i] == s.clauses[expected_reasons[i]]);
        }
    }
    //CHECK(s.antecedent_clauses == expected_reasons);
}