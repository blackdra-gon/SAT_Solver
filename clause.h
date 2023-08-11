//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_CLAUSE_H
#define SAT_SOLVER_CLAUSE_H

#include <utility>
#include <vector>
#include <optional>
#include <memory>
#include "solver_structs.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

class ClauseRef;
class Solver;


class Clause : public std::enable_shared_from_this<Clause>{
public:
    explicit Clause(std::vector<Literal_t> literals, bool learnt= false): literals(std::move(literals)), learnt(learnt) {
        activity = 1;
    }

    std::vector<Literal_t> literals;
    bool learnt;
    double activity;
    /**
     * only for learnt clauses: return if clause is reason for a current assignment (and therefore should not be deleted)
     * @param other
     * @return
     */
    bool locked(Solver &s);

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
    /**
     * Is called only as part of preprocessing. Deletes false literals
     * @return true, if a literal in the clause is already true, therefore the clause can be deleted
     */
    bool simplify(Solver &s);

    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & literals;
        ar & activity;
        ar & learnt;
    }


};



#endif //SAT_SOLVER_CLAUSE_H
