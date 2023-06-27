//
// Created by benjamin on 27.06.23.
//

#ifndef SAT_SOLVER_CLAUSEREF_H
#define SAT_SOLVER_CLAUSEREF_H

#include <functional>
#include "clause.h"

class ClauseRef {
    std::reference_wrapper<Clause> ref;
    static Clause* clauses_base_address;
public:
    static Clause *getClausesBaseAddress();
    ClauseIndex_t index;

public:
    static void setClausesBaseAddress(Clause *clausesBaseAddress);

    ClauseRef(std::reference_wrapper<Clause>);
    Clause& get() const;
    bool operator==(const Clause& other) const;

    friend std::ostream& operator<<(std::ostream& os, const ClauseRef& output);
};




#endif //SAT_SOLVER_CLAUSEREF_H
