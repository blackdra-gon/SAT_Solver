//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_SOLVER_H
#define SAT_SOLVER_SOLVER_H

#include <vector>
#include <queue>
#include <optional>
#include <functional>
#include <memory>
#include "solver_structs.h"
#include "clause.h"
#include "clauseRef.h"
#include "variable_order.h"

using Watchlists = std::vector<std::vector<std::reference_wrapper<Clause>>>;


class Solver {
public:
    /**
     * function to be called after importing clauses to solve the formular
     * @return true: satisfiable, false: unsatisfiable
     */
    bool solve();

    /**
     * is called internaly by solve() to look for a solution. Might return UNASSIGNED, when restarts are implemented
     * @return
     */
    lbool search(uint32_t number_of_conflicts, uint32_t maximum_learnt_clauses);


    std::vector<std::shared_ptr<Clause>> clauses;
    std::vector<std::shared_ptr<Clause>> learnt_clauses;
    std::queue<Literal_t> propagation_queue;
    std::vector<std::vector<std::weak_ptr<Clause>>> watch_lists; // literal indexed




    // Assignments
    std::vector<lbool> assignments; // variable indexed
    std::vector<Literal_t> trail;
    /**
     * trail_limits[i] contains the number of entries the trail had, before the assume of current_decision_level i + 1 was
     * conducted.
     */
    std::vector<int> trail_limits;
    std::vector<std::optional<std::weak_ptr<Clause>>> antecedent_clauses; // variable indexed
    std::vector<int> decision_levels; // variable indexed

    // Variable ordering
    VariableOrder variableOrder;
    std::vector<double> var_activities; // variable indexed
    void bumpVariable(Variable_t var);
    void decayActivities();
    // To be replaced with VariableOrder
    Literal_t next_unassigned_variable();

    // Clause activities
    void bumpClause(const std::shared_ptr<Clause>& clause);

    lbool value(Literal_t);
public:
    void analyse_conflict(std::shared_ptr<Clause> conflicting_clause, std::vector<Literal_t> &out_learnt, int& out_bt_level);
    /**
     *
     * @param literals
     * @param learnt
     * @return true on success, false if an added unit clause causes a conflict
     */
    bool addClause(const std::vector<Literal_t> &literals, bool learnt= false);

    /**
     *
     * @param clauses in dimacs representation
     * @return true on success, false when an added unit clause causes a conflict
     */
    bool addClauses(const std::vector<std::vector<int>>& clauses);
    void setNumberOfVariables(int number);
    /**
     * Add a unit constraint to the propagation queue and also add the respective assignment
     * @param literal
     * @param reason clause, which implied this unit constraint
     * @return true on succes, false if a conflict was detected
     */
    bool enqueue(Literal_t literal, const std::optional<std::shared_ptr<Clause>>& reason=std::nullopt);
    /**
     * Propagates all entries of the propagation queue to all clauses on the watchlist of the respective literal.
     * In case of a conflict, returns the clause which causes the conflict.
     */
    std::optional<std::shared_ptr<Clause>> propagate();

    int current_decision_level() const;

    bool assume(Literal_t literal);
    void pop_trail();
    void backtrack_until(int level);
    void backtrack_one_level();
    void record_learnt_clause( const std::vector<Literal_t>& clause);

    void print_clauses();
    friend class VariableOrder;

    long index_of_highest_decision_level(Clause &clause);

    void reduce_learnt_clauses();
};

#endif //SAT_SOLVER_SOLVER_H
