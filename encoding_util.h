//
// Created by benjamin on 10.05.23.
//

#ifndef SAT_SOLVER_ENCODING_UTIL_H
#define SAT_SOLVER_ENCODING_UTIL_H

#include "solver_structs.h"

Variable_t negate_literal(Variable_t literal);

Variable_t internal_representation(int input);

int dimacs_format(Variable_t internal_variable);

#endif //SAT_SOLVER_ENCODING_UTIL_H
