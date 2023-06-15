//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_CLAUSE_H
#define SAT_SOLVER_CLAUSE_H

#include <vector>
#include "solver_structs.h"


class Clause {
public:
    std::vector<Literal_t> literals;

    bool operator==(const Clause& other) const;
    /**
     * This function is used to propagate the effects of the assignment l to this clause.
     * This clause is in the watchlist of l, which means that it contains the literal not-l as a watched literal (Index 0 or 1).
     * @param s
     * @param l
     * @return false, if conflict occurs. true on succes
     */
    //bool propagate(const Solver& s, Literal_t l);
};

std::ostream& operator<<(std::ostream& os, const Clause& output);

#endif //SAT_SOLVER_CLAUSE_H
