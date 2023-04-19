#include <iostream>
#include "lorina/dimacs.hpp"
#include "lorina/diagnostics.hpp"


class reader : public lorina::dimacs_reader
{
public:
    void on_format(const std::string &format) const override
    {
        std::cout << format << std::endl;
    };

    void on_number_of_clauses(uint64_t number_of_clauses) const override
    {

    }

    void on_number_of_variables(uint64_t number_of_variables) const override
    {

    }
    void on_clause(const std::vector<int> &clause) const override
    {
        std::cout << "clause" << std::endl;
    }
    void on_end() const override
    {

    }


};

/*class diagnostics : public lorina::diagnostic_consumer
{
public:
    void handle_diagnostic( lorina::diagnostic_level level, std::string const& message ) const override
    {
        std::cout << message << std::endl;
    }
}; /* diagnostics */

int main() {
    //diagnostics consumer;
    //lorina::diagnostic_engine diag( &consumer );
    lorina::return_code returnCode = lorina::read_dimacs("../example_dimacs/example1.cnf", reader());
    if (returnCode == lorina::return_code::success)
        std::cout << "success" << std::endl;
    else
        std::cout << "error" << std::endl;
    return 0;
}

/*
 * on_format(format)


Callback method for parsed format (CNF or DNF)

on_number_of_variables(n)


Callback method for parsed number of variables

on_number_of_clauses(n)


Callback method for parsed number of clauses

on_clause(clause)


Callback method for parsed clauses

on_end()


Callback method for parsed end
 */
