//
// Created by benjamin on 26.04.23.
//

#ifndef SAT_SOLVER_IO_UTILS_H
#define SAT_SOLVER_IO_UTILS_H

#include <string>
#include <vector>
#include <bitset>

std::string output_dimacs(std::vector<bool>);

std::vector<bool> bitset_to_vector(std::bitset<64> bitset);

#endif //SAT_SOLVER_IO_UTILS_H
