//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_CLAUSE_H
#define SAT_SOLVER_CLAUSE_H

#include <utility>
#include <vector>
#include <optional>
#include "solver_structs.h"

class ClauseRef;
class Solver;


class Clause {
public:
    explicit Clause(std::vector<Literal_t> literals, bool learnt= false): literals(std::move(literals)), learnt(learnt) {}

    std::vector<Literal_t> literals;
    bool learnt;

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
    /**
     *
     * @param s Solver
     * @param l either this->literals[0] or std::nullopt
     * @param out_reason return all literals, which are reason for the conflict (the negation of all literals of the clause), or
     * which are reason for l to be assigned (the negation of all literals of the clause but l)
     */
    void calc_reason(Solver& s, std::optional<Literal_t> l, std::vector<Literal_t> &out_reason);

    friend std::ostream& operator<<(std::ostream& os, const Clause& output);
};



#endif //SAT_SOLVER_CLAUSE_H
