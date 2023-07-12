#include <iostream>

#include "lorina_include/lorina/dimacs.hpp"

#include "io_utils.h"
#include "solver_structs.h"
#include "encoding_util.h"
#include "dpll.h"


int main(int argc, char** argv) {
    Solver s;
    std::string input_file;
    // Read the file given as first positional argument, use stdin if no argument given
    if ( argc > 1 ) {
        input_file = argv[1];
        try {
            import_from_file(input_file, s);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }

    // Get input from stdin
    } else {
        auto parse_cnf_result = lorina::read_dimacs(std::cin, Reader(s));
        if (parse_cnf_result == lorina::return_code::parse_error) {
            std::cerr << "Lorina parse error, when trying to parse from stdin" << std::endl;
            return 1;
        }

    }
    std::cout << "succesfully imported instance" << std::endl;
    //s.print_clauses();
    if (s.solve()) {
        std::cout << "SATISFIABLE" << std::endl;
        if (!input_file.empty()) {
            output_model_to_file(s.assignments, input_file);
        }
    } else {
        std::cout << "UNSATISFIABLE" << std::endl;
    }

    //s.print_clauses();



    return 0;
}

