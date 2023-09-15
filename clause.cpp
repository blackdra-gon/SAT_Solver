//
// Created by benjamin on 13.06.23.
//

#include <iostream>
#include <algorithm>
#include "clause.h"
#include "solver.h"
#include "encoding_util.h"
#include "clauseRef.h"

bool Clause::operator==(const Clause &other) const{
    return literals == other.literals;
}

bool Clause::propagate(Solver& s, Literal_t l) {
    // false literal shall be literals[1]
    if (literals[0] == negate_literal(l)) {
        literals[0] = literals[1];
        literals[1] = negate_literal(l);
    }
    // if 0th watch is true, reinsert to watchlist of l
    if (s.value(literals[0]) == TRUE) {
        s.watch_lists[l].emplace_back(weak_from_this());
        return true;
    }
    // Look for new literal to watch
    for (int i = 2; i < literals.size(); ++i) {
        if (s.value(literals[i]) != FALSE) {
            literals[1] = literals[i];
            literals[i] = negate_literal(l);
            s.watch_lists[negate_literal(literals[1])].emplace_back(weak_from_this());
            return true;
        }
    }
    // Clause is unit now
    // std::cout << "Literal " << dimacs_format(literals[0]) << " is unit now in " << *this << std::endl;
    s.watch_lists[l].emplace_back(weak_from_this());
    return s.enqueue(literals[0], shared_from_this());
}



std::ostream &operator<<(std::ostream &os, const Clause &output) {
    return os << output.literals;
}





bool Clause::operator==(const ClauseRef &other) const {
    return *this == other.get();
}

void Clause::calc_reason(Solver &s, std::optional<Literal_t> l, std::vector<Literal_t> &out_reason) {
    int i = l ? 1 : 0;
    for (; i < literals.size(); ++i) {
        out_reason.push_back(negate_literal(literals[i]));
    }
    if (learnt) {
        s.bumpClause(shared_from_this());
    }
}

bool Clause::locked(Solver &s) {
    auto reason_for_first_literal = s.antecedent_clauses[var_index(literals[0])];
    if (reason_for_first_literal.has_value()) {
        return reason_for_first_literal.value().lock() == shared_from_this();
    }
    return false;
}

bool Clause::simplify(Solver &s) {
    int j = 0;
    for (auto literal : literals) {
        if (s.value(literal) == TRUE) {
            //std::cout << dimacs_format(literal) << ": " << *this << std::endl;
            return true;
        } else if (s.value(literal) == UNASSIGNED) {
            literals[j++] = literal;
        } //else {
            //std::cout << "erased literal " << dimacs_format(literal) << std::endl;
        //}
    }
#if COLLECT_SOLVER_STATISTICS
    s.solver_stats.statistics["literals_deleted_during_preprocessing"] += literals.size() - j;
#endif
    literals.erase(std::next(literals.begin(), j), literals.end());
    return false;
}
