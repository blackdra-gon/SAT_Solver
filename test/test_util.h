#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "../solver.h"

//
// Created by benjamin on 21.06.23.
//
void check_assignments(Solver &s, std::vector<lbool> expected_values) {
    CHECK(s.assignments == expected_values);
}


// Ideas for making this check weaker: do not check the order inside a watchlist; just check, if the expected clauses
// are a subset of the clause actually in the watchlist
/**
 *
 * @param s                 solver
 * @param expected_clauses  Takes the indices of the clause in the watchlist as parameter. Uses negative values for the
 *                          indices of learnt clauses (starts to count them by 1: -1 means learned_clauses[0]
 */
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
            int clause_index = expected_clauses[i][j];
            if (clause_index >= 0) {
                CHECK(s.watch_lists[i][j].lock() == s.clauses[clause_index]);
            } else {
                CHECK(s.watch_lists[i][j].lock() == s.learned_clauses[-clause_index - 1]);
            }
        }
    }
}

void check_clauses(Solver &s, const std::vector<std::vector<int>>& expected_clauses, bool learnt= false) {
    std::vector<Clause> expected;
    for (const auto& clause: expected_clauses) {
        expected.emplace_back(internal_representation(clause));
    }
    int i = 0;
    auto clause_vector = learnt ? s.learned_clauses : s.clauses;
    CHECK(clause_vector.size() == expected.size());
    for (const auto& clause_ptr: clause_vector) {
        CHECK(*clause_ptr <= expected[i]);
        CHECK( expected[i] <= *clause_ptr);
        ++i;
    }
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
            CHECK(s.antecedent_clauses[i].value().lock() == s.clauses[expected_reasons[i]]);
        }
    }
    //CHECK(s.antecedent_clauses == expected_reasons);
}