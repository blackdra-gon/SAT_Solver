//
// Created by benjamin on 10.05.23.
//

#include "encoding_util.h"

Literal_t negate_literal(Literal_t literal) {
    return literal ^ 1;
}

Literal_t internal_representation(int input) {
    input *= 2;
    if (input < 0) {
        input = input * -1 + 1;
    }
    input -= 2;
    return input;
}

int dimacs_format(Variable_t internal_variable) {
    int output = int(internal_variable);
    output += 2;
    if (output % 2 == 1) {
        output -= 1;
        output *= -1;
    }
    output /= 2;
    return output;
}

Clause internal_representation(const std::vector<int>& input) {
    Clause output = {};
    for (int x: input) {
        output.literals.push_back(internal_representation(x));
    }
    return output;
}

std::vector<Clause> internal_representation(const std::vector<std::vector<int>> &input) {
    std::vector<Clause> output;
    for (auto x: input) {
        output.push_back(internal_representation(x));
    }
    return output;
}




