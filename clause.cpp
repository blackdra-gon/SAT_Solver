//
// Created by benjamin on 13.06.23.
//

#include "clause.h"

bool Clause::operator==(const Clause &other) const{
    return literals == other.literals;
}

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
