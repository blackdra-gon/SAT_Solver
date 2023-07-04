//
// Created by benjamin on 04.07.23.
//

#ifndef SAT_SOLVER_VARIABLE_ORDER_H
#define SAT_SOLVER_VARIABLE_ORDER_H


#include "solver_structs.h"

class VariableOrder {
public:
    Literal_t select_next();
};

#endif //SAT_SOLVER_VARIABLE_ORDER_H
