//
// Created by benjamin on 13.06.23.
//

#include <iostream>
#include <functional>
#include "solver.h"
#include "clause.h"

void Solver::addClause(const Clause& clause) {
    if (clause.literals.empty()) {
        std::cout << "WARNING: Tried to add empty clause" << std::endl;
    } else if (clause.literals.size() == 1) {
        propagation_queue.push(clause.literals.back());
    } else {
        clauses.push_back(clause);
    }
}

bool Solver::enqueue(Literal_t l, Clause &reason) {
    return false;
}

lbool Solver::value(Literal_t l) {
    return assignments[l >> 1];
}

void Solver::setNumberOfVariables(int number) {
    for (; number > 0; --number) {
        assignments.push_back(UNASSIGNED);
        watch_lists.emplace_back();
        watch_lists.emplace_back();
    }

}
