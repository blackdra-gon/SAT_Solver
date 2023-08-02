//
// Created by benjamin on 27.04.23.
//

#ifndef SAT_SOLVER_SOLVER_STRUCTS_H
#define SAT_SOLVER_SOLVER_STRUCTS_H

#include <vector>
#include <cstdint>
#include <ostream>

#define DECISION_LEVEL_UNASSIGNED (-2)

using Variable_t = uint32_t;
using ClauseIndex_t = uint32_t;

using Literal_t = uint32_t;

Variable_t var_index(Literal_t);
bool sign(Literal_t);



enum lbool: uint8_t {
    TRUE, FALSE, UNASSIGNED
};

lbool operator!(lbool b);

lbool lsign(Literal_t);

Literal_t negative_literal(Variable_t var_index);

Literal_t positive_literal(Variable_t var_index);

std::ostream &operator<<(std::ostream &os, const std::vector<Literal_t> &output);

bool operator<=(const std::vector<Literal_t> & a, const std::vector<Literal_t> & b);








#endif //SAT_SOLVER_SOLVER_STRUCTS_H
