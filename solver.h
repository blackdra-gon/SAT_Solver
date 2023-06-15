//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_SOLVER_H
#define SAT_SOLVER_SOLVER_H

#include <vector>
#include <queue>
#include "solver_structs.h"
#include "clause.h"


class Solver {
public:


    std::vector<Clause> clauses;
    std::queue<Literal_t> propagation_queue;
    std::vector<std::vector<std::reference_wrapper<Clause>>> watch_lists; // literal indexed
    std::vector<lbool> assignments; // variable indexed
    std::vector<Literal_t> trail;
    std::vector<int> decision_levels;
    std::vector<std::reference_wrapper<Clause>> antecedent_clauses;

    lbool value(Literal_t);
public:
    void addClause(const Clause&);
    void setNumberOfVariables(int number);
    bool enqueue(Literal_t l, Clause& reason);
};

#endif //SAT_SOLVER_SOLVER_H
