#include <iostream>
#include "lorina/dimacs.hpp"
#include "lorina/diagnostics.hpp"
#include "io_utils.h"
#include <cxxopts.hpp>



using Clause = std::vector<unsigned int>;

struct Cnf {
    std::vector<Clause> clauses;
    int number_of_variables;
};

const int MAX_NUMBER_OF_VARIABLES = 64;


class Reader : public lorina::dimacs_reader
{
private:
    Cnf &cnf;

public:
    Reader(Cnf &cnf) : cnf(cnf) {}

    void on_format(const std::string &format) const override
    {
        std::cout << format << std::endl;
    };

    void on_number_of_clauses(uint64_t number_of_clauses) const override
    {

    }

    void on_number_of_variables(uint64_t number_of_variables) const override
    {
        cnf.number_of_variables = number_of_variables;
    }
    void on_clause(const std::vector<int> &clause_input) const override
    {
        Clause clause_store;

        for (int literal: clause_input) {
            literal *= 2;
            if (literal < 0) {
                literal = literal * -1 + 1;
            }
            literal -= 2;
            clause_store.push_back(static_cast<unsigned int>(literal));
        }
        cnf.clauses.push_back(clause_store);


    }
    void on_end() const override
    {

    }


};


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
    cxxopts::Options options("SAT_Solver", "A sat-solver programmed for a TCS Seminar by Benjamin Hennies ");
    options.add_options()
            ("file", "file containing a cnf formula in dimacs format", cxxopts::value<std::string>());
    options.parse_positional("file");
    auto result = options.parse(argc, argv);
    if ( result.count("file") ) {
        std::string input_file = result["file"].as<std::string>();
        auto parse_cnf_result = lorina::read_dimacs(input_file, Reader(cnf));
        if (parse_cnf_result == lorina::return_code::parse_error) {
            std::cout << "Lorina parse error, when trying to parse " << input_file << std::endl;
            return 1;
        }
    // Get input from stdin
    } else {
        auto parse_cnf_result = lorina::read_dimacs(std::cin, Reader(cnf));
        if (parse_cnf_result == lorina::return_code::parse_error) {
            std::cout << "Lorina parse error, when trying to parse from stdin" << std::endl;
            return 1;
        }

    }



    for (const auto& clause : cnf.clauses) {
        std::cout << "clause" << std::endl;
        for (auto literal : clause) {
            std::cout << literal << " ";
        }
        std::cout << std::endl;
    }
    solve(cnf);
    return 0;
}

