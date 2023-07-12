//
// Created by benjamin on 26.04.23.
//

#ifndef SAT_SOLVER_IO_UTILS_H
#define SAT_SOLVER_IO_UTILS_H

#include <string>
#include <vector>
#include <bitset>


#include "solver_structs.h"
#include "encoding_util.h"
#include "solver.h"

void import_from_file(std::string, Solver&);

void output_model_to_file(const std::vector<lbool>& assignments, const std::string& input_file_name);

#endif //SAT_SOLVER_IO_UTILS_H
