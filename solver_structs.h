//
// Created by benjamin on 27.04.23.
//

#ifndef SAT_SOLVER_SOLVER_STRUCTS_H
#define SAT_SOLVER_SOLVER_STRUCTS_H

#include <vector>
#include <cstdint>

using Clause = std::vector<uint64_t>;

struct Assignment {
    uint64_t variable_index_value;
    bool tried_the_other_value;
    bool implied;
    uint64_t decision_level;
    uint64_t antecedent_clause;

    explicit Assignment(uint64_t variable_index_value_,
               bool implied_ = true,
               bool tried_the_other_value_ = false,
               uint64_t decision_level_ = 0,
               uint64_t antecedent_clause_ = 0)
            : variable_index_value(variable_index_value_),
              tried_the_other_value(tried_the_other_value_),
              implied(implied_),
              decision_level(decision_level_),
              antecedent_clause(antecedent_clause_)
    {}
};

using Assignments = std::vector<Assignment>;

struct Cnf {
    std::vector<Clause> clauses;
    int number_of_variables;
    Assignments assignments;

    bool operator==(Cnf const& rhs) const {
        return clauses == rhs.clauses;
    }
};



#endif //SAT_SOLVER_SOLVER_STRUCTS_H
