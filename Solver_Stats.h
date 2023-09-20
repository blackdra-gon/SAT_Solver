//
// Created by benjamin on 14.09.23.
//

#ifndef SAT_SOLVER_SOLVER_STATS_H
#define SAT_SOLVER_SOLVER_STATS_H


#include <cstdint>
#include <string>
#include <map>

class Solver_Stats {
public:
    std::map<std::string, int64_t> statistics{
            {"number_of_conflicts",                                     0},
            {"number_of_decisions",                                     0},
            {"number_of_propagated_literals",                           0},
            {"number_of_restarts",                                      0},
            {"number_of_deleted_clauses",                               0},

            {"clauses_deleted_during_preprocessing",                    0},
            {"literals_deleted_during_preprocessing",                   0},

            {"clauses_deleted_during_inprocessing_subsumption",         0},
            // ssr = self-subsuming resolution
            {"literals_deleted_from_newly_learned_clauses_with_ssr",    0},
            {"literals_deleted_from_recently_learned_clauses_with_ssr", 0},
            {"literals_not_deleted_because_of_watchlist",               0},
            {"subset_check_fast", 0},
            {"subset_check_slow", 0}

    };

    void print_statistics();
};


#endif //SAT_SOLVER_SOLVER_STATS_H
