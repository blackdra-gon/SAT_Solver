//
// Created by benjamin on 27.04.23.
//
#include <algorithm>
#include <vector>
#include <iostream>

#include "dpll.h"
#include "encoding_util.h"

/*auto find_unit_clause(const Cnf &cnf) {
    return std::find_if(cnf.clauses.begin(),
                        cnf.clauses.end(),
                        [](const Clause &c) { return c.size() == 1; });
}

bool apply_unit_propagation(Cnf &cnf) {
    auto unit_clause_it = find_unit_clause(cnf);
    while (unit_clause_it != cnf.clauses.end()) {
        // Assign the value obtained by the unit clause
        uint64_t unit_clause_variable = unit_clause_it->at(0);
        Assignment new_assignment(unit_clause_variable);
        cnf.assignments.push_back(new_assignment);
        std::cout << "Assigned implied variable " << dimacs_format(unit_clause_variable) << std::endl;
        // For all clauses: delete all clause which have a true literal now
        auto erased_clauses = std::erase_if(cnf.clauses, [unit_clause_variable](const Clause &clause) {
                                                return std::ranges::find(clause, unit_clause_variable) != clause.end();
                                            }
        );
        std::cout << "Removed " << erased_clauses << " clauses, because they are satisfied now" << std::endl;
        // delete the false literal from all clauses
        for (Clause &clause: cnf.clauses) {
            if (std::erase(clause, negate_literal(unit_clause_variable)) > 0) {
                std::cout << "Removed " << dimacs_format(negate_literal(unit_clause_variable)) << " from a clause" << std::endl;
            }
            if (clause.empty()) {
                std::cout << "Conflict! Empty Clause" << std::endl;
                return false;
            }
        }
        // Find next unitclause
        unit_clause_it = find_unit_clause(cnf);
    }
    return true;
}

uint32_t choose_next_variable(Cnf &cnf) {
    for (int i = 1; i <= cnf.number_of_variables; ++i) {
        if (std::ranges::find_if(cnf.assignments, [i](const Assignment a) {
            return a.variable_index_value == internal_representation(i)
                   || a.variable_index_value == internal_representation(-i);
        }) == cnf.assignments.end()) {
            return internal_representation(i);
        }
    }
    std::cout << "All variables are already assigned" << std::endl;
    return 0;
}

bool dpll_recursive(Cnf &cnf) {
    if (!apply_unit_propagation(cnf)) {
        std::cout << "UNSAT" << std::endl;
        return false;
    }
    if (cnf.clauses.empty()) {
        std::cout << cnf.assignments << std::endl;
        return true;
    }
    uint32_t next_variable_for_branching = choose_next_variable(cnf);
    std::cout << "Decision variable: " << dimacs_format(next_variable_for_branching) << std::endl;
    Cnf branch(cnf);
    branch.clauses.push_back({next_variable_for_branching});
    // The assignment will be added in the unit propagation step
    // branch.assignments.emplace_back(next_variable_for_branching);
    if (dpll_recursive(branch)) {
        std::cout << branch.assignments  << std::endl;
        return true;
    } else {
        // Pop assignments until branching variables appears
        while (branch.assignments.back().variable_index_value != next_variable_for_branching) {
            branch.assignments.pop_back();
        }
        branch.assignments.pop_back();
        branch.clauses.pop_back();
        branch.clauses.push_back({negate_literal(next_variable_for_branching)});
        std::cout << "Backtracking: Instead of " << dimacs_format(next_variable_for_branching) << "choose" << dimacs_format(
                negate_literal(next_variable_for_branching))  << std::endl;
        if (dpll_recursive(branch)) {
            std::cout << branch.assignments  << std::endl;
            return true;
        }

    }
    std::cout << "UNSAT"  << std::endl;
    return false;
}*/
