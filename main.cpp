#include <iostream>

#include "io_utils.h"


int main(int argc, char** argv) {
    Solver s;
    std::string input_file;
    bool trivial_conflict;
    // Read the file given as first positional argument, use stdin if no argument given
    if ( argc > 1 ) {
        input_file = argv[1];

        try {
            trivial_conflict = !import_from_file(input_file, s);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }

    // Get input from stdin
    } else {
        std::cout << "Input form stdin is not implemented" << std::endl;
        return 1;

    }
    // std::cout << "succesfully imported instance" << std::endl;
    if (trivial_conflict || !s.preprocess()) {
        std::cout << "UNSATISFIABLE" << std::endl;
        return 0;
    }
    //s.print_clauses();
    if (s.solve()) {
        std::cout << "SATISFIABLE" << std::endl;
        if (!input_file.empty()) {
            output_model_to_file(s.assignments, input_file);
        }
    } else {
        std::cout << "UNSATISFIABLE" << std::endl;
    }
#if COLLECT_SOLVER_STATISTICS
    s.solver_stats.print_statistics();
#endif

    //s.print_clauses();



    return 0;
}

