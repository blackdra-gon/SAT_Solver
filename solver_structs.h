//
// Created by benjamin on 27.04.23.
//

#ifndef SAT_SOLVER_SOLVER_STRUCTS_H
#define SAT_SOLVER_SOLVER_STRUCTS_H

#include <vector>

using Clause = std::vector<unsigned int>;

struct Cnf {
    std::vector<Clause> clauses;
    int number_of_variables;
};

#endif //SAT_SOLVER_SOLVER_STRUCTS_H
