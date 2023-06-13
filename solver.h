//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_SOLVER_H
#define SAT_SOLVER_SOLVER_H

#include <vector>
#include "clause.h"

class Solver {
    std::vector<Clause> clauses;
    std::vector<lbool> assignments;
    std::vector<Literal_t> trail;
    std::vector<int> decision_levels;
    std::vector<Clause*> antecedent_clauses;
};

#endif //SAT_SOLVER_SOLVER_H
