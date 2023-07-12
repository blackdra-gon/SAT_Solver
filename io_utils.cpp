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
    std::ifstream in( filename, std::ifstream::in );
    if ( !in.is_open() )
    {
        std::cout << "Error: Could not open file" << std::endl;
    }
    uint number_of_variables;
    uint number_of_clauses;
    std::string line;
    std::string literal_str;
    // Reading first line, ignoring comments
    do {
        std::getline(in, line);
    } while (line[0] == 'c');

    int i = 0;
    std::stringstream stringStream(line);
    while (std::getline(stringStream, literal_str, ' ')) {
        switch (i) {
            case 0:
                if (literal_str != "p") {
                    std::cout << "parsing error" << std::endl;
                }
                break;
            case 1:
                if (literal_str != "cnf") {
                    std::cout << "parsing error" << std::endl;
                }
                break;
            case 2:
                number_of_variables = std::stoi(literal_str);
                solver.setNumberOfVariables(number_of_variables);
                break;
            case 3:
                number_of_clauses = std::stoi(literal_str);
                solver.clauses.reserve(number_of_clauses);
                solver.learnt_clauses.reserve(number_of_clauses*2);
                break;
            default:
                std::cout << "parsing error" << std::endl;
                break;

        }
        ++i;
    }
    while (std::getline(in, line)) {
        std::stringstream stringStream(line);
        std::vector<Literal_t> clause;
        while (std::getline(stringStream, literal_str, ' ')) {
            int literal = std::stoi(literal_str);
            if (literal != 0) {
                clause.push_back(internal_representation(literal));
            }
        }
        solver.addClause(clause);

    }
}

void output_model_to_file(const std::vector<lbool>& assignments, const std::string& input_file_name) {
    std::ofstream output_file;
    std::string output_file_name = input_file_name + ".sol";
    output_file.open(output_file_name);
    int number_of_variables = assignments.size();
    if (output_file.is_open()) {
        output_file << "p sat " << number_of_variables << " " << number_of_variables << std::endl;
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
