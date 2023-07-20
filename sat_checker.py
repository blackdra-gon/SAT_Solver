# from https://github.com/kelson-martins/sat-problem-checker.git

import sys
import re

PROBLEM_FILEPATH = sys.argv[1]
SOLUTION_FILEPATH = sys.argv[2]

def read_clauses(path):

    clauses = []

    try:
        file = open(path,"r")

        clauses_lines = False

        for line in file:
            if line.startswith("p"):
                clauses_lines = True
                continue

            if line.startswith("%"):
                clauses_lines = False

            if clauses_lines:
                # removing trailing spaces, removing last 0 items and converting to number
                clause = [int(s) for s in re.split('[\s]+',line.strip())][:-1]

                clauses.append(clause)

        file.close()

    except IOError:
        print("Can not find input file or read data")
        exit()

    return clauses

def read_solution(path):

    solution = []

    try:
        file = open(path,"r")

        for line in file:
            if line.startswith("p"):
                continue
                #split_solution = line[3:].split(" ")

                #for i in range(len(split_solution)):
                #    solution.append(split_solution[i])
            if line.isnumeric():
                solution.append(line)

        # converting list to int and removing last item "0"
        solution = list(map(int, solution))

        file.close()

    except IOError:
        print("Can not find input file or read data")
        exit()

    return solution

def check_satisfation(clauses, solution):

    total_clauses = len(clauses)
    total_satistied = 0
    total_non_satisfied = 0

    # iterating over all clauses
    for i in range(len(clauses)):

        satistied = False

        for j in range(len(clauses[i])):

            if clauses[i][j] in solution:
                satistied = True

        if satistied:
            total_satistied += 1
        else:
            total_non_satisfied += 1

    print("Total Clauses: " + str(total_clauses))
    print("Satisfied Clauses: " + str(total_satistied))
    print("Non Satisfied Clauses: " + str(total_non_satisfied))

def main():

    clauses = read_clauses(PROBLEM_FILEPATH)
    solution = read_solution(SOLUTION_FILEPATH)

    check_satisfation(clauses,solution)


if __name__ == "__main__":
    main()