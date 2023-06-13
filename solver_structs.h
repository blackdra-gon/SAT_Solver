//
// Created by benjamin on 27.04.23.
//

#ifndef SAT_SOLVER_SOLVER_STRUCTS_H
#define SAT_SOLVER_SOLVER_STRUCTS_H

#include <vector>
#include <cstdint>
#include <ostream>

using Variable_t = uint32_t;
using ClauseIndex_t = uint32_t;

struct Literal_t {
    Variable_t literal;
};

enum lbool: uint8_t {
    TRUE, FALSE, UNASSIGNED
};

//using Clause = std::vector<Variable_t>;

struct Assignment {
    Variable_t variable_index_value;
    bool tried_the_other_value;
    bool implied;
    Variable_t decision_level;
    ClauseIndex_t antecedent_clause;

    explicit Assignment(Variable_t variable_index_value_,
                        bool implied_ = true,
                        bool tried_the_other_value_ = false,
                        Variable_t decision_level_ = 0,
                        ClauseIndex_t antecedent_clause_ = 0)
            : variable_index_value(variable_index_value_),
              tried_the_other_value(tried_the_other_value_),
              implied(implied_),
              decision_level(decision_level_),
              antecedent_clause(antecedent_clause_)
    {}

    bool operator==(Assignment const& rhs) const {
        return variable_index_value == rhs.variable_index_value;
    }

};

using Assignments = std::vector<Assignment>;

std::ostream& operator<<(std::ostream& os, const Assignment& output);

std::ostream& operator<<(std::ostream& os, const Assignments& output);


Assignments from_dimacs_list(const std::vector<int>& assignment_list);

/*struct Cnf {
    std::vector<Clause> clauses;
    int number_of_variables;
    Assignments assignments;

    bool operator==(Cnf const& rhs) const {
        return clauses == rhs.clauses;
    }

};*/

//std::ostream& operator<<(std::ostream& os, const Cnf& output);








#endif //SAT_SOLVER_SOLVER_STRUCTS_H
