//
// Created by benjamin on 13.05.23.
//
#include <iostream>
#include "solver_structs.h"
#include "encoding_util.h"

Variable_t var_index(Literal_t l) {
    return l >> 1;
}

Literal_t positive_literal(Variable_t var_index) {
    return var_index << 1;
}

Literal_t negative_literal(Variable_t var_index) {
    return (var_index << 1) + 1;
}
/**
 *
 * @param l
 * @return true, if literal is positive, false if it has a negation
 */
bool sign(Literal_t l) {
    return !(l % 2);
}

lbool lsign(Literal_t l) {
    return sign(l) ? TRUE : FALSE;
}

lbool operator!(lbool b) {
    return b == TRUE ? FALSE : b == FALSE ? TRUE : UNASSIGNED;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Literal_t> &output) {
    os << "{";
    for (auto l: output) {
        if (l == output.back()) {
            os << dimacs_format(l);
        } else {
            os << dimacs_format(l) << ", ";
        }
    }
    os << "}";
    return os;
}

bool operator<=(const std::vector<Literal_t> &a, const std::vector<Literal_t> &b) {
    for (auto literal: a) {
        if (std::find(b.begin(), b.end(), literal) == b.end()) {
            return false;
        }
    }
    return true;
}

