//
// Created by benjamin on 19.07.23.
//

#include <iostream>
#include "../clause.h"
#include "../solver.h"
#include "../io_utils.h"

int main(int argc, char** argv) {
    Solver problem;
    import_from_file(argv[1], problem);
    Solver solution;
    import_from_file(argv[2], solution);
    for (const auto& clause: problem.clauses) {
        bool sat_flag = false;
        for (auto literal: clause->literals) {
            if (literal % 2 == 0) {
                if (solution.assignments[var_index(literal)] == TRUE) {
                    sat_flag = true;
                    break;
                }
            } else {
                if (solution.assignments[var_index(literal)] == FALSE) {
                    sat_flag = true;
                    break;
                }
            }
        }
        if (!sat_flag) {
            std::cout << "None of the literals in clause " << *clause << " evaluates to true" << std::endl;
            return 1;
        }
    }
    std::cout << "Valid solution" << std::endl;
    return 0;


}
