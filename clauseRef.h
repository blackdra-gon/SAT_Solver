//
// Created by benjamin on 27.06.23.
//

#ifndef SAT_SOLVER_CLAUSEREF_H
#define SAT_SOLVER_CLAUSEREF_H

#include <functional>
#include "clause.h"

class ClauseRef {
    std::reference_wrapper<Clause> ref;

public:
    ClauseRef(std::reference_wrapper<Clause>);
    Clause get();
}

#endif //SAT_SOLVER_CLAUSEREF_H
