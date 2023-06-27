//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_CLAUSE_H
#define SAT_SOLVER_CLAUSE_H

#include <vector>
#include "solver_structs.h"

class ClauseRef;
class Solver;


class Clause {
public:
    std::vector<Literal_t> literals;

    bool operator==(const Clause& other) const;
    bool operator==(const ClauseRef& other) const;
    /**
     * This function is used to propagate the effects of the assignment l to this clause.
     * This clause is in the watchlist of l, which means that it contains the literal not-l as a watched literal (Index 0 or 1).
     * The watchlist of l was moved to a temporary vector before, so this clause is not in the watchlist of l (but may
     * be reinserted during this function.
     * @param s
     * @param l
     * @return false, if conflict occurs. true on succes
     */
    bool propagate(Solver& s, Literal_t l);

    friend std::ostream& operator<<(std::ostream& os, const Clause& output);
};



#endif //SAT_SOLVER_CLAUSE_H
