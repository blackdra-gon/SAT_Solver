//
// Created by benjamin on 14.09.23.
//

#ifndef SAT_SOLVER_SOLVER_STATS_H
#define SAT_SOLVER_SOLVER_STATS_H


#include <cstdint>

class Solver_Stats {
public:
    uint64_t number_of_conflicts = 0;
    uint64_t number_of_decisions = 0;
    uint64_t number_of_propagated_literals = 0;
    uint64_t number_of_restarts = 0;
    uint64_t number_of_deleted_clauses = 0;

    uint64_t clauses_deleted_during_preprocessing = 0;
    uint64_t literals_deleted_during_preprocessing = 0;

    uint64_t clauses_deleted_during_inprocessing_subsumption = 0;
    // ssr = self-subsuming resolution
    uint64_t literals_deleted_from_newly_learned_clauses_with_ssr = 0;
    uint64_t literals_deleted_from_recently_learned_clauses_with_ssr = 0;
    uint64_t literals_not_deleted_because_of_watchlist = 0;


};


#endif //SAT_SOLVER_SOLVER_STATS_H
