#include "io_utils.h"

std::string output_dimacs(std::vector<bool> assignment) {
    int n = assignment.size();
    std::string result = "v ";
    for (int i = 1; i <= n; i++) {
        if (assignment[i-1]) {
            result += std::to_string(i);
        } else {
            result += "-" + std::to_string(i);
        }
        if (i < n) {
            result += " ";
        }
    }
    return result;
}

std::vector<bool> bitset_to_vector(std::bitset<64> bitset) {
    std::vector<bool> bool_vector;
    for (int i = 0; i < bitset.size(); i++) {
        bool_vector.push_back(bitset.test(i));
    }
    return bool_vector;
}

Cnf import_from_file(std::string filename) {
    Cnf cnf;
    auto parse_cnf_result = lorina::read_dimacs(filename, Reader(cnf));
    if (parse_cnf_result == lorina::return_code::parse_error) {
        throw std::runtime_error("Lorina parse error, when trying to parse " + filename);
    }
    return cnf;
}
