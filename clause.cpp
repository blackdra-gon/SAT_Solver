//
// Created by benjamin on 13.06.23.
//

#include <iostream>
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
    auto clause_ref_this = ClauseRef(std::ref(*this));
    if (s.value(literals[0]) == TRUE) {
        s.watch_lists[l].emplace_back(clause_ref_this);
        return true;
    }
    // Look for new literal to watch
    for (int i = 2; i < literals.size(); ++i) {
        if (s.value(literals[i]) != FALSE) {
            literals[1] = literals[i];
            literals[i] = negate_literal(l);
            s.watch_lists[negate_literal(literals[1])].emplace_back(clause_ref_this);
            return true;
        }
    }
    // Clause is unit now
    std::cout << "Literal " << dimacs_format(literals[0]) << " is unit now in " << clause_ref_this << std::endl;
    s.watch_lists[l].emplace_back(clause_ref_this);
    return s.enqueue(literals[0], clause_ref_this);
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
    // Bump clause activity for learnt clauses
}
