//
// Created by benjamin on 13.06.23.
//

#include "clause.h"

bool Clause::operator==(const Clause &other) const{
    return literals == other.literals;
}

//bool Clause::propagate(const Solver& s, Literal_t l) {
//    return false;
//}

std::ostream &operator<<(std::ostream &os, const Clause &output) {
    os << "{";
    for (auto l: output.literals) {
        if (l == output.literals.back()) {
            os << l;
        } else {
            os << l << ", ";
        }
    }
    os << "}";
    return os;
}
