//
// Created by benjamin on 10.05.23.
//

#include "encoding_util.h"

Variable_t negate_literal(Variable_t literal) {
    return literal ^ 1;
}

Variable_t internal_representation(int input) {
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


