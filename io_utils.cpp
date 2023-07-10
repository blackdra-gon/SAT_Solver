#include "io_utils.h"
#include <fstream>

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

void import_from_file(std::string filename, Solver &solver) {
    auto parse_cnf_result = lorina::read_dimacs(filename, Reader(solver));
    if (parse_cnf_result == lorina::return_code::parse_error) {
        throw std::runtime_error("Lorina parse error, when trying to parse " + filename);
    }
}

void output_model_to_file(const std::vector<lbool>& assignments, const std::string& input_file_name) {
    std::ofstream output_file;
    std::string output_file_name = input_file_name + ".sol";
    output_file.open(output_file_name);
    int number_of_variables = assignments.size();
    if (output_file.is_open()) {
        output_file << "p sat" << number_of_variables << " " << number_of_variables << std::endl;
        for (int i = 1; i <= number_of_variables; ++i) {
            if (assignments[i-1] == TRUE) {
                output_file << i << std::endl;
            } else if (assignments[i-1] == FALSE) {
                output_file << -i << std::endl;
            } else {
                std::cerr << "Tried to export incomplete model";
            }
        }
    } else {
        std::cerr << "Could not open file" << std::endl;
    }
    output_file.close();
}
