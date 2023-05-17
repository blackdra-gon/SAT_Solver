//
// Created by benjamin on 13.05.23.
//
#include <iostream>
#include "solver_structs.h"
#include "encoding_util.h"

std::ostream& operator<<(std::ostream& os, const Cnf& cnf) {
    for (const auto& clause : cnf.clauses) {
        for (auto literal : clause) {
            os << dimacs_format(literal) << " ";
        }
        os << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Assignment& output) {
    os << dimacs_format(output.variable_index_value);
    return os;
}

Assignments from_dimacs_list(const std::vector<int>& assignment_list) {
    Assignments assignments;
    for (auto assignment_value: assignment_list) {
        assignments.emplace_back(internal_representation(assignment_value));
    }
    return assignments;
}
