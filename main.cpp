#include <iostream>

#include "lorina_include/lorina/dimacs.hpp"

#include "io_utils.h"
#include "solver_structs.h"
#include "encoding_util.h"
#include "dpll.h"

const int MAX_NUMBER_OF_VARIABLES = 64;

bool literalSatisfied(unsigned int literal, const std::bitset<MAX_NUMBER_OF_VARIABLES> &assignment) {
    unsigned int variable_index = literal >> 1;
    bool positive_literal = literal % 2 == 0;
    bool var_assignment = assignment[variable_index];
    return var_assignment == positive_literal;
}

bool checkAssignment(const std::vector<Clause>& clauses, const std::bitset<MAX_NUMBER_OF_VARIABLES> &assignment) {
    for (const auto& clause : clauses) {
        bool clauseSatisfied = false;
        for (auto literal : clause) {

            if (literalSatisfied(literal, assignment)) {
                clauseSatisfied = true;
            }

        }
        if (!clauseSatisfied) { return false; }
    }
    return true;
}

void solve(Cnf &cnf) {
    for (unsigned long long i = 0; i < 1 << cnf.number_of_variables; i++) {
        std::bitset<MAX_NUMBER_OF_VARIABLES> possible_solution(i);
        //std::cout << possible_solution << std::endl;
        if (checkAssignment(cnf.clauses, possible_solution)) {
            std::cout << "SAT" << std::endl;
            std::cout << possible_solution << std::endl;
            std::cout << output_dimacs(bitset_to_vector(possible_solution)) << std::endl;
            return;
        }
    }
    std::cout << "UNSAT" << std::endl;
}


int main(int argc, char** argv) {
    Cnf cnf;
    // Read the file given as first positional argument, use stdin if no argument given
    if ( argc > 1 ) {
        std::string input_file = argv[1];
        try {
            cnf = import_from_file(input_file);
        } catch (std::exception &e) {
            std::cerr << e.what();
            return 1;
        }

    // Get input from stdin
    } else {
        auto parse_cnf_result = lorina::read_dimacs(std::cin, Reader(cnf));
        if (parse_cnf_result == lorina::return_code::parse_error) {
            std::cerr << "Lorina parse error, when trying to parse from stdin" << std::endl;
            return 1;
        }

    }



    for (const auto& clause : cnf.clauses) {
        std::cout << "clause" << std::endl;
        for (auto literal : clause) {
            std::cout << dimacs_format(literal) << " ";
        }
        std::cout << std::endl;
    }
    dpll_recursive(cnf);
    return 0;
}

