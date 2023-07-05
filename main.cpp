#include <iostream>

#include "lorina_include/lorina/dimacs.hpp"

#include "io_utils.h"
#include "solver_structs.h"
#include "encoding_util.h"
#include "dpll.h"


int main(int argc, char** argv) {
    Solver s;
    // Read the file given as first positional argument, use stdin if no argument given
    if ( argc > 1 ) {
        std::string input_file = argv[1];
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
    s.print_clauses();
    s.search();
    s.print_clauses();



    return 0;
}

