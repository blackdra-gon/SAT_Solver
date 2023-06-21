//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_SOLVER_H
#define SAT_SOLVER_SOLVER_H

#include <vector>
#include <queue>
#include <optional>
#include <functional>
#include "solver_structs.h"
#include "clause.h"


class Solver {
public:


    std::vector<Clause> clauses;
    std::queue<Literal_t> propagation_queue;
    std::vector<std::vector<std::reference_wrapper<Clause>>> watch_lists; // literal indexed




    // Assignments
    std::vector<lbool> assignments; // variable indexed
    std::vector<Literal_t> trail;
    std::vector<int> trail_limits;
    std::vector<std::optional<std::reference_wrapper<Clause>>> antecedent_clauses; // variable indexed
    std::vector<int> decision_levels; // variable indexed

    lbool value(Literal_t);
public:
    void addClause(const Clause&);
    void setNumberOfVariables(int number);
    /**
     * Add a unit constraint to the propagation queue and also add the respective assignment
     * @param literal
     * @param reason clause, which implied this unit constraint
     * @return true on succes, false if a conflict was detected
     */
    bool enqueue(Literal_t literal, std::optional<std::reference_wrapper<Clause>> reason=std::nullopt);
};

#endif //SAT_SOLVER_SOLVER_H
