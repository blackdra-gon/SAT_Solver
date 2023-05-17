//
// Created by benjamin on 27.04.23.
//
#include <algorithm>
#include <vector>

#include "easylogging++.h"

#include "dpll.h"
#include "encoding_util.h"

auto find_unit_clause(const Cnf& cnf) {
    return std::find_if(cnf.clauses.begin(),
                       cnf.clauses.end(),
                       [](const Clause& c) { return c.size() == 1; });
}

bool apply_unit_propagation(Cnf& cnf) {
    auto unit_clause_it = find_unit_clause(cnf);
    while (unit_clause_it != cnf.clauses.end()) {
        // Assign the value obtained by the unit clause
        uint64_t unit_clause_variable = unit_clause_it->at(0);
        Assignment new_assignment(unit_clause_variable);
        cnf.assignments.push_back(new_assignment);
        LOG(INFO) << "Assigned implied variable " << dimacs_format(unit_clause_variable);
        // For all clauses: delete all clause which have a true literal now
        auto erased_clauses = std::erase_if(cnf.clauses, [unit_clause_variable](const Clause &clause) {
                                                return std::ranges::find(clause, unit_clause_variable) != clause.end();
                                            }
        );
        LOG(INFO) << "Removed " << erased_clauses << " clauses, because they are satisfied now";
        // delete the false literal from all clauses
        for (Clause &clause: cnf.clauses) {
            std::erase(clause, negate_literal(unit_clause_variable));
            LOG(INFO) << "Removed " << negate_literal(unit_clause_variable) << " from a clause";
            if (clause.empty()) {
                LOG(INFO) << "Conflict! Empty Clause";
            }
        }
        // Find next unitclause
        unit_clause_it = find_unit_clause(cnf);
    }
}
