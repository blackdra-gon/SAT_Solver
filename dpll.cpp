//
// Created by benjamin on 27.04.23.
//
#include <algorithm>
#include "easylogging++.h"
#include "dpll.h"

auto find_unit_clause(const Cnf& cnf) {
    return std::find_if(cnf.clauses.begin(),
                       cnf.clauses.end(),
                       [](const Clause& c) { return c.size() == 1; });
}

Cnf apply_unit_propagation(Cnf& cnf) {
    auto unit_clause_it = find_unit_clause(cnf);
    while (unit_clause_it != cnf.clauses.end()) {
        // Assign the value obtained by the unit clause
        uint64_t unit_clause_variable = unit_clause_it->at(0);
        Assignment new_assignment(unit_clause_variable);
        cnf.assignments.push_back(new_assignment);
        LOG(INFO) << "Assigned implied variable " << unit_clause_variable;
        // For all clauses: delete all clause which have a true literal now
        // delete the false literal from all clauses
        // TODO: apply erase on cnf.clauses
        for (auto clause: cnf.clauses) {
            clause.erase(std::remove_if(clause.begin(),
                            clause.end(),
                            [unit_clause_variable](uint64_t i) {return i == unit_clause_variable; }),
                         clause.end());
        }

    }
    return cnf;
}
