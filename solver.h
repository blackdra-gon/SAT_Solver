//
// Created by benjamin on 13.06.23.
//

#ifndef SAT_SOLVER_SOLVER_H
#define SAT_SOLVER_SOLVER_H

#define COLLECT_SOLVER_STATISTICS true

#define LOG_SEARCH true

#include <vector>
#include <queue>
#include <optional>
#include <functional>
#include <memory>
#include "solver_structs.h"
#include "clause.h"
#include "clauseRef.h"
#include "variable_order.h"
#if COLLECT_SOLVER_STATISTICS
    #include "Solver_Stats.h"

    extern Solver_Stats solverStats;
#endif

using Watchlists = std::vector<std::vector<std::reference_wrapper<Clause>>>;

using Clause_ptr = std::shared_ptr<Clause>;
using Clause_wptr = std::weak_ptr<Clause>;


class Solver {
public:
    /**
     * function to be called after importing clauses to solve the formular
     * @return true: satisfiable, false: unsatisfiable
     */
    bool solve();

    /**
     * is called internally by solve() to look for a solution. Might return UNASSIGNED, when restarts are implemented
     * @return
     */
    lbool search(uint32_t number_of_conflicts, uint32_t maximum_learnt_clauses);


    std::vector<std::shared_ptr<Clause>> clauses;
    std::vector<std::shared_ptr<Clause>> learnt_clauses;
    std::queue<Literal_t> propagation_queue;
    std::vector<std::vector<std::weak_ptr<Clause>>> watch_lists; // literal indexed
    std::vector<std::vector<Clause_wptr>> occurrence_lists; // literal indexed, for preprocessing only, should be deleted before search




    // Assignments
    std::vector<lbool> assignments; // variable indexed
    std::vector<Literal_t> trail;
    /**
     * Assign a value to a variable. To be used during preprocessing. In search, use Solver::enqueue()
     * @param var
     * @param value
     */
    void assign(Literal_t lit);
    /**
     * trail_limits[i] contains the number of entries the trail had, before the assume of current_decision_level i + 1 was
     * conducted.
     */
    std::vector<int> trail_limits;
    //TODO: Replace optional with empty weak pointers here
    std::vector<std::optional<std::weak_ptr<Clause>>> antecedent_clauses; // variable indexed
    std::vector<int> decision_levels; // variable indexed

    // Variable ordering
    std::vector<double> var_activities; // variable indexed
    void bumpVariable(Variable_t var);
    void decayActivities();
    // To be replaced with VariableOrder
    Literal_t next_unassigned_variable();

    // Clause activities
    void bumpClause(const std::shared_ptr<Clause>& clause);

    lbool value(Literal_t l) const;
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
    bool addClauses(const std::vector<std::vector<int>>& clauses, bool learnt= false);
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
    void record_learnt_clause( std::vector<Literal_t> &clause);

    void print_clauses();
    friend class VariableOrder;

    long index_of_highest_decision_level(Clause &clause);

    void reduce_learnt_clauses();
    /**
     * performs unitpropagation, then deletes satisfied clauses und false literals
     * should only be called before solving
     * @return true on success, false if a conflict occurred
     */
    bool preprocess();
    /**
     * finds literals, whose negated form does not occur in the clause. Thus it can be safely assigned and all clauses
     * which contain this literal, are deleted, because they will never be useful for unit propagation
     */
    void pure_literal_elimination();
    /**
     *
     * @param var variable to perform resolution on
     * @param out_eliminated_var used to return whether or not var is eliminated
     * @param bound_factor number of clauses after elimination may exceed the number of clauses before by bound_factor.
     *                     Otherwise, the operation is cancelled
     * @return false, if a conflict was detected in the meantime, true on success
     */
    bool maybe_eliminate(Variable_t var, bool &out_eliminated_var, uint bound_factor = 1);
    /**
     *
     * @param positive_occurrence occurrence list
     * @param negative_occurrence occurrence list
     * @param out_eliminated_var used to return whether or not var is eliminated
     * @param max_resolvents maximum number of resolvents, which is generated before this try of variable elimination is aborted
     * @return false, if a conflict was detected in the meantime, true on success
     */
    bool maybe_eliminate(std::vector<Clause_wptr> &positive_occurrence, std::vector<Clause_wptr> &negative_occurrence,
                         bool &out_eliminated_var, uint max_resolvents);
    /**
     * @pre no unit literals in clauses. Otherwise it would be possible to derive the empty clause in this step
     * @param a contains a literal var
     * @param b contains a literal not(var) (or the other way round)
     * @param var variable which will be resolved away
     * @return Vector of literals. If the vector is empty, the resolvent was a tautology
     */
    std::vector<Literal_t> resolve(const Clause_ptr &a, const Clause_ptr& b, Variable_t var);
    void delete_occurrence_lists();
#if COLLECT_SOLVER_STATISTICS
    Solver_Stats solver_stats;
#endif
private:
    bool contains_true_literal(const std::shared_ptr<Clause>& clause);


};

#endif //SAT_SOLVER_SOLVER_H
