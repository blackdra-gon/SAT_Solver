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

/**
 *
 * @param filename relative or absolute path to import
 * @param solver Solver to import to
 * @return true on success, false on conflicting unit clauses.
 * @throws std::runtime_error on IO failures (mostly wrong filename)
 */
bool import_from_file(std::string filename, Solver &solver);

void output_model_to_file(const std::vector<lbool>& assignments, const std::string& input_file_name);

#endif //SAT_SOLVER_IO_UTILS_H
