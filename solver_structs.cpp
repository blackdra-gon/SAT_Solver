//
// Created by benjamin on 13.05.23.
//
#include <iostream>
#include "solver_structs.h"
#include "encoding_util.h"

/*std::ostream& operator<<(std::ostream& os, const Cnf& cnf) {
    for (const auto& clause : cnf.clauses) {
        for (auto literal : clause) {
            os << dimacs_format(literal) << " ";
        }
        os << std::endl;
    }
    return os;
}*/

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

std::ostream &operator<<(std::ostream &os, const Assignments &output) {
    for (auto assingment: output) {
        os << assingment << " ";
    }
    return os;
}

Variable_t var_index(Literal_t l) {
    return l >> 1;
}

Literal_t positive_literal(Variable_t var_index) {
    return var_index << 1;
}

Literal_t negative_literal(Variable_t var_index) {
    return (var_index << 1) + 1;
}
/**
 *
 * @param l
 * @return true, if literal is positive, false if it has a negation
 */
bool sign(Literal_t l) {
    return !(l % 2);
}

lbool lsign(Literal_t l) {
    return sign(l) ? TRUE : FALSE;
}

lbool operator!(lbool b) {
    return b == TRUE ? FALSE : b == FALSE ? TRUE : UNASSIGNED;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Literal_t> &output) {
    os << "{";
    for (auto l: output) {
        if (l == output.back()) {
            os << dimacs_format(l);
        } else {
            os << dimacs_format(l) << ", ";
        }
    }
    os << "}";
    return os;
}

