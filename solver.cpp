//
// Created by benjamin on 13.06.23.
//

#include <iostream>
#include <functional>
#include "solver.h"
#include "clause.h"
#include "encoding_util.h"


void Solver::addClause(const Clause& clause) {
    if (clause.literals.empty()) {
        std::cout << "WARNING: Tried to add empty clause" << std::endl;
    } else if (clause.literals.size() == 1) {
        enqueue(clause.literals.back());
    } else {
        clauses.push_back(clause);
        // Add clause to the watchlist of the negation of the two first elements
        watch_lists[negate_literal(clause.literals[0])].push_back(std::ref(clauses.back()));
        watch_lists[negate_literal(clause.literals[1])].push_back(std::ref(clauses.back()));
    }
}

bool Solver::enqueue(Literal_t literal, std::optional<std::reference_wrapper<Clause>> reason) {
    if (value(literal) != UNASSIGNED) {
        if (value(literal) == FALSE) {
            // Conflicting assignment found
            while (!propagation_queue.empty()) {
                propagation_queue.pop();
            }
            return false;
        } else {
            // This variable was already assigned
            return true;
        }
    } else {
        assignments[var_index(literal)] = lsign(literal);
        trail.push_back(literal);
        // level[var_index(literal)] = currentDecisionLevel();
        antecedent_clauses[var_index(literal)] = reason;
        propagation_queue.push(literal);
        return true;
    }
}

lbool Solver::value(Literal_t l) {
    if (sign(l)) {
        return assignments[var_index(l)];
    } else {
        return !assignments[var_index(l)];
    }
}

void Solver::setNumberOfVariables(int number) {
    assignments.clear();
    decision_levels.clear();
    antecedent_clauses.clear();
    watch_lists.clear();
    assignments.reserve(number);
    decision_levels.reserve(number);
    antecedent_clauses.reserve(number);
    watch_lists.reserve(2*number);
    for (; number > 0; --number) {
        assignments.push_back(UNASSIGNED);
        decision_levels.push_back(DECISION_LEVEL_UNASSIGNED);
        antecedent_clauses.emplace_back();
        watch_lists.emplace_back();
        watch_lists.emplace_back();
    }

}

void Solver::propagate() {
    while (!propagation_queue.empty()) {
        Literal_t literal = propagation_queue.front();
        propagation_queue.pop();
        std::vector<std::reference_wrapper<Clause>> tmp_watchlist;
        tmp_watchlist = watch_lists[literal];
        watch_lists[literal].clear();
        //std::move(watch_lists[literal].begin(), watch_lists[literal].end(), tmp_watchlist.begin());
        for (auto watched_clause: tmp_watchlist) {
            watched_clause.get().propagate(*this, literal);
        }

    }
}
/**
 * Should only be called once for every solver object, otherwise not enough space is reserved
 * @param clauses
 */
void Solver::addClauses(std::vector<std::vector<int>> input_clauses) {
    clauses.reserve(input_clauses.size());
    for (auto literal_list: input_clauses) {
        addClause(internal_representation(literal_list));
    }
}
