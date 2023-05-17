//
// Created by benjamin on 27.04.23.
//

#ifndef SAT_SOLVER_DPLL_H
#define SAT_SOLVER_DPLL_H

#include "solver_structs.h"
/*
 * Returns true, when unit propagation was successfull without conflict.
 * Reurns false, when a conflict was detected
 */
bool apply_unit_propagation(Cnf &cnf);


#endif //SAT_SOLVER_DPLL_H
