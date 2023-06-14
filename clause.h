//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_CLAUSE_H
#define SAT_SOLVER_CLAUSE_H

#include <vector>
#include "solver_structs.h"

class Clause {
public:
    std::vector<Literal_t> literals;

    bool operator==(const Clause& other) const;
};

std::ostream& operator<<(std::ostream& os, const Clause& output);

#endif //SAT_SOLVER_CLAUSE_H
