//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_CLAUSE_H
#define SAT_SOLVER_CLAUSE_H

#include <vector>
#include "solver_structs.h"

class Clause {
    std::vector<Literal_t> literals;
};

#endif //SAT_SOLVER_CLAUSE_H
