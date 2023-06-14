//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_SOLVER_H
#define SAT_SOLVER_SOLVER_H

#include <vector>
#include <queue>
#include "clause.h"

class Solver {
public:


    std::vector<Clause> clauses;
    std::queue<Literal_t> propagation_queue;
private:
    std::vector<lbool> assignments;
    std::vector<Literal_t> trail;
    std::vector<int> decision_levels;
    std::vector<Clause*> antecedent_clauses;
public:
    void addClause(const Clause&);
};

#endif //SAT_SOLVER_SOLVER_H
