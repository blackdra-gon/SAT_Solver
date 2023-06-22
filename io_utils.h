//
// Created by benjamin on 26.04.23.
//

#ifndef SAT_SOLVER_IO_UTILS_H
#define SAT_SOLVER_IO_UTILS_H

#include <string>
#include <vector>
#include <bitset>

#include "lorina_include/lorina/dimacs.hpp"

#include "solver_structs.h"
#include "encoding_util.h"
#include "solver.h"

class Reader : public lorina::dimacs_reader
{
private:
    Solver &solver;

public:
    explicit Reader(Solver &solver) : solver(solver) {}

    void on_format(const std::string &format) const override {};

    void on_number_of_clauses(uint64_t number_of_clauses) const override
    {
        solver.clauses.reserve(number_of_clauses);
    }

    void on_number_of_variables(uint64_t number_of_variables) const override
    {
        solver.setNumberOfVariables(number_of_variables);

        // reserve also the other vectors?
    }
    void on_clause(const std::vector<int> &clause_input) const override
    {
        Clause clause_store;

        for (int literal: clause_input) {
            clause_store.literals.push_back(internal_representation(literal));
        }
        solver.addClause(clause_store);


    }
    void on_end() const override
    {

    }


};

void import_from_file(std::string, Solver&);

std::string output_dimacs(std::vector<bool>);

std::vector<bool> bitset_to_vector(std::bitset<64> bitset);

#endif //SAT_SOLVER_IO_UTILS_H
