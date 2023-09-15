//
// Created by benjamin on 14.09.23.
//

#include <iostream>
#include "Solver_Stats.h"

void Solver_Stats::print_statistics() {
    for (const auto& [key, value] : statistics) {
        std::cout << key << ",";
    }
    std::cout << std::endl;
    for (const auto& [key, value] : statistics) {
        std::cout << value << ",";
    }

}
