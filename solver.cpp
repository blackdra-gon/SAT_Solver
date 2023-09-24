//
// Created by benjamin on 13.06.23.
//

#include <iostream>
#include <functional>
#include <cassert>
#include <algorithm>
#include <chrono>
#include <set>
#include "solver.h"
#include "clause.h"
#include "encoding_util.h"

#if COLLECT_SOLVER_STATISTICS
Solver_Stats solverStats;
#endif

bool Solver::addClause(const std::vector<Literal_t> &literals, bool learnt) {
    if (literals.empty()) {
        std::cout << "WARNING: Tried to add empty clause" << std::endl;
    } else if (literals.size() == 1) {
        // returns false on a conflicting unit clause
        if (!enqueue(literals.back())) {
            return false;
        }
    } else {
        // allocate clause
        auto clause = std::make_shared<Clause>(literals, learnt);
        // select vector to append to
        auto &clause_vector = learnt ? learned_clauses : clauses;
        clause_vector.push_back(clause);

        // Set BaseAddress for debugging purposes
        /*if (clauses.size() == 1) {
            ClauseRef::setClausesBaseAddress(&clauses[0]);
        }
        if (learned_clauses.size() == 1) {
            ClauseRef::setLearntClausesBaseAddress(&learned_clauses[0]);
        }*/
        if (learnt) {
            // pick second watch literal with the highest decision level
            auto i = index_of_highest_decision_level(*clause);
            clause->literals[1] = clause->literals[i];
            clause->literals[i] = literals[1];
            // Bumping
            for (auto literal: literals) {
                bumpVariable(var_index(literal));
            }
        } else {
            // Add clause to occurrencelists for preprocessing
                //for (auto literal: clause->literals) {
                //    occurrence_lists[literal].emplace_back(clause);
                //}
        }
        // Add clause to the watchlist of the negation of the two first elements
        watch_lists[negate_literal(clause->literals[0])].emplace_back(clause);
        watch_lists[negate_literal(clause->literals[1])].emplace_back(clause);

    }
    return true;
}

bool Solver::enqueue(Literal_t literal, const std::optional<std::shared_ptr<Clause>>& reason) {
    if (value(literal) != UNASSIGNED) {
        if (value(literal) == FALSE) {
            // Conflicting assignment found
            while (!propagation_queue.empty()) {
                propagation_queue.pop();
            }
            return false;
        } else {
            // This variable was already assigned
            return true;
        }
    } else {
        assign(literal);
        antecedent_clauses[var_index(literal)] = reason;
        propagation_queue.push(literal);
        return true;
    }
}

lbool Solver::value(Literal_t l) const {
    if (sign(l)) {
        return assignments[var_index(l)];
    } else {
        return !assignments[var_index(l)];
    }
}

void Solver::setNumberOfVariables(int number) {
    assignments.clear();
    decision_levels.clear();
    antecedent_clauses.clear();
    watch_lists.clear();
    var_activities.clear();
    assignments.reserve(number);
    decision_levels.reserve(number);
    antecedent_clauses.reserve(number);
    watch_lists.reserve(2*number);
    var_activities.reserve(number);
    for (; number > 0; --number) {
        assignments.push_back(UNASSIGNED);
        decision_levels.push_back(DECISION_LEVEL_UNASSIGNED);
        antecedent_clauses.emplace_back();
        watch_lists.emplace_back();
        watch_lists.emplace_back();
        occurrence_lists.emplace_back();
        occurrence_lists.emplace_back();
        var_activities.push_back(1);
    }

}

std::optional<std::shared_ptr<Clause>> Solver::propagate() {
    while (!propagation_queue.empty()) {
#if COLLECT_SOLVER_STATISTICS
        ++solverStats.statistics["number_of_propagated_literals"];
#endif
        Literal_t literal = propagation_queue.front();
#if LOG_SEARCH
        std::cout << "LEVEL " << current_decision_level() << ": Propagating " << dimacs_format(literal) << std::endl;
#endif
        propagation_queue.pop();
        auto tmp_watchlist = watch_lists[literal];
        watch_lists[literal].clear();
        //std::move(watch_lists[literal].begin(), watch_lists[literal].end(), tmp_watchlist.begin());
        for (int i = 0; i < tmp_watchlist.size(); ++i) {
            auto clause = tmp_watchlist[i].lock();
            if (!clause) {
                // The learnt clause referenced here was deleted in the meantime
                // the associated weak pointer is now deleted, because it is not reinserted from the tmp_watchlist
                continue;
            }
            if(!clause->propagate(*this, literal)) {
                // Conflict occurred
                // reinsert remaining entries from tmp_watchlist
                for (int j = i + 1; j < tmp_watchlist.size(); ++j) {
                    watch_lists[literal].push_back(tmp_watchlist[j]);
                }
                return clause;
            }
        }

    }
    return std::nullopt;
}
/**
 * Should only be called once for every solver object. Used only for testing.
 * @param clauses
 */
bool Solver::addClauses(const std::vector<std::vector<int>>& clauses, bool learnt) {
    //clauses.reserve(input_clauses.size());
    learned_clauses.reserve(clauses.size() * 10);
    for (const auto& literal_list: clauses) {
        if (!addClause(internal_representation(literal_list), learnt)) {
            return false;
        }
    }
    return true;
}

int Solver::current_decision_level() const {
    return trail_limits.size();
}

bool Solver::assume(Literal_t literal) {
    trail_limits.push_back(trail.size());
    return enqueue(literal);
}

// Warning: changes the content of conflicting clause
void Solver::analyse_conflict(std::shared_ptr<Clause> conflicting_clause, std::vector<Literal_t> &out_learnt, int& out_bt_level) {
    std::vector<bool> seen(assignments.size(), false);
    int counter = 0;
    std::optional<Literal_t> p = std::nullopt;
    std::vector<Literal_t> p_reason;

    // reserve the first entry for literal of current decision level.
    out_learnt.push_back(UINT32_MAX);
    out_bt_level = 0;
    do {
        p_reason.clear();
        conflicting_clause->calc_reason(*this, p, p_reason);
        // Trace reason for p
        for (int j = 0; j < p_reason.size(); j++) {
            Literal_t q = p_reason[j];
            if (!seen[var_index(q)]) {
                seen[var_index(q)] = true;
                if (decision_levels[var_index(q)] == current_decision_level()) {
                    ++counter;
                } else { // check for decision level > 0 ?
                    out_learnt.push_back(negate_literal(q));
                    out_bt_level = std::max(out_bt_level, decision_levels[var_index(q)]);
                }
            }
        }
        // Go back the trail to select next literal to look at
        do {
            p = trail.back();
            auto antecedent_clause = antecedent_clauses[var_index(p.value())];
            if (antecedent_clause) {
                // learnt clauses which are antecedent clause are locked and should not be deleted
                assert(antecedent_clause.value().lock());
                conflicting_clause = antecedent_clause.value().lock();
            } else {
                // decision variable of current level found: No need to go further
                --counter;
                assert(counter == 0);
            }

            pop_trail();
        } while (!seen[var_index(p.value())]);
        --counter;
    } while (counter > 0);
    out_learnt[0] = negate_literal(p.value());

}

void Solver::pop_trail() {
    Literal_t p = trail.back();
    Variable_t x = var_index(p);
    assignments[x] = UNASSIGNED;
    antecedent_clauses[x] = std::nullopt;
    decision_levels[x] = DECISION_LEVEL_UNASSIGNED;
    // order.undo(x);
    trail.pop_back();
}

void Solver::backtrack_until(int level) {
    while (current_decision_level() > level) {
        backtrack_one_level();
    }
}

void Solver::backtrack_one_level() {
    int steps = trail.size() - trail_limits.back();
    for (; steps != 0; --steps) {
        pop_trail();
    }
    trail_limits.pop_back();
}

void Solver::record_learnt_clause(std::vector<Literal_t> &clause) {
    // Temporary clause for signature subsumption test
    Clause new_clause(clause);
    uint nof_clauses_to_lock_back = std::min(size_t(LOOKBACK_N_CLAUSES), learned_clauses.size());
    auto index_to_look_back = learned_clauses.size() - nof_clauses_to_lock_back;
    for (auto i = learned_clauses.size(); i > index_to_look_back; --i) {
        auto &learned_clause = learned_clauses[i - 1];
        // Try Backward Subsumption on the last learnt clauses
        if (new_clause <= *learned_clause) {
#if LOG_SEARCH
            std::cout << "Deleting subsumed Clause: " << *learned_clause << std::endl;
#endif
            std::erase(learned_clauses, learned_clause);
#if COLLECT_SOLVER_STATISTICS
            ++solverStats.statistics["clauses_deleted_during_inprocessing_subsumption"];
#endif
        }
    }
    // Would it be possible that selfsubsuming resolution deletes the last literal? No
    assert(!clause.empty());
    if (clause.size() > 1) {
        addClause(clause, true);
        enqueue(clause[0], learned_clauses.back());
    } else {
        //std::cout << "Learned clause with only one literal" << std::endl;
        enqueue(clause[0]);
    }
}

bool Solver::solve() {
    lbool status = UNASSIGNED;
    double number_of_conflicts_until_restart = 100;
    double max_learnt_clauses = assignments.size() / 3;
    while (status == UNASSIGNED) {
        status = search(int(number_of_conflicts_until_restart), int(max_learnt_clauses));
        number_of_conflicts_until_restart *= 1.5;
        max_learnt_clauses *= 1.1;
#if COLLECT_SOLVER_STATISTICS
        ++solverStats.statistics["number_of_restarts"];
#endif
    }
    return status == TRUE;
}

lbool Solver::search(uint32_t number_of_conflicts, uint32_t maximum_learnt_clauses) {
    int conflict_counter = 0;
    while (true) {
        auto conflicting_clause = propagate();
        if (conflicting_clause) {
            // Conflict handling
            ++conflict_counter;
#if COLLECT_SOLVER_STATISTICS
            ++solverStats.statistics["number_of_conflicts"];
#endif
#if LOG_SEARCH
            std::cout << "Conflict in " << *conflicting_clause.value() << std::endl;
#endif
            if (current_decision_level() == 0) {
                return FALSE;
            }
            std::vector<Literal_t> learnt_clause;
            int backtrack_level = 0;
            analyse_conflict(conflicting_clause.value(), learnt_clause, backtrack_level);
#if LOG_SEARCH
            std::cout << "Backtrack to level " << backtrack_level << std::endl;
            std::cout << "Learnt clause: " << learnt_clause << std::endl;
#endif
            backtrack_until(backtrack_level);
            record_learnt_clause(learnt_clause);
            decayActivities();
        } else {
            // No conflict
            if (trail.size() == assignments.size()) {
                // All variables are assigned
#if LOG_SEARCH
                std::cout << "Satisfying assignment found: " << trail << std::endl;
#endif
                return TRUE;
            }
            // learnt clauses reach capacity
            if (learned_clauses.size() >= maximum_learnt_clauses) {
                reduce_learnt_clauses();
            }
            if (conflict_counter >= number_of_conflicts) {
                backtrack_until(0);
                return UNASSIGNED;
            }
            // Take new assumption
            Literal_t next_assumption = next_unassigned_variable();
#if LOG_SEARCH
            std::cout << "LEVEL " << current_decision_level() + 1 << ": Assuming " << dimacs_format(next_assumption) << std::endl;
#endif
            assume(next_assumption);
#if COLLECT_SOLVER_STATISTICS
            ++solverStats.statistics["number_of_decisions"];
#endif
        }
    }
}

Literal_t Solver::next_unassigned_variable() {
    double max_activity = 0;
    Variable_t max_index = UINT32_MAX;
    for (int i = 0; i < assignments.size(); ++i) {
        if (assignments[i] == UNASSIGNED) {
            if (var_activities[i] > max_activity) {
                max_activity = var_activities[i];
                max_index = i;
            }
        }
    }
    return max_index << 1;
}

void Solver::print_clauses() {
    for (const auto& clause: clauses) {
        std::cout << *clause << std::endl;
    }
    if (!learned_clauses.empty()) {
        std::cout << "Learnt Clauses:" << std::endl;
        for (const auto& clause: learned_clauses) {
            std::cout << *clause << std::endl;
        }
    }
}

long Solver::index_of_highest_decision_level(Clause &clause) {
    auto level_compare = [this](Literal_t a, Literal_t b)
            { return (decision_levels[var_index(a)] < decision_levels[var_index(b)]); };
    auto result = std::ranges::max_element(clause.literals, level_compare);
    return std::ranges::distance(clause.literals.begin(), result);
}

void Solver::bumpVariable(Variable_t var) {
    var_activities[var] += 1;
}

void Solver::decayActivities() {
    for (auto &activity_value: var_activities) {
        activity_value *= 0.95;
    }
    for (const auto& clause: learned_clauses) {
        clause->activity *= 0.999;
    }
}

void Solver::bumpClause(const std::shared_ptr<Clause>& clause) {
    clause->activity += 1;
}

void Solver::reduce_learnt_clauses() {
    // std::cout << "Reduce set of learnt Clauses" << std::endl;
    std::ranges::sort(learned_clauses, [](const std::shared_ptr<Clause>& a, const std::shared_ptr<Clause>& b) {
        return a->activity > b->activity;
    });
    auto size_before = learned_clauses.size();
    size_t middle = size_before / 2;
    auto half_learnt_clause = std::next(learned_clauses.begin(), middle);
    auto result = std::remove_if(half_learnt_clause, learned_clauses.end(), [this](const std::shared_ptr<Clause>& c) {
        return !c->locked(*this);
    });
    learned_clauses.erase(result, learned_clauses.end());
#if COLLECT_SOLVER_STATISTICS
    solverStats.statistics["number_of_deleted_clauses"] += size_before - learned_clauses.size();
#endif
}

bool Solver::preprocess() {
    if (!propagation_queue.empty()) {
        if (propagate() != std::nullopt) {
            return false;
        }
        auto erased_clauses = std::erase_if(clauses, [this](auto clause) { return clause->simplify(*this); });
#if COLLECT_SOLVER_STATISTICS
        solverStats.statistics["clauses_deleted_during_preprocessing"] = erased_clauses;
#endif
    }
    std::cout << "finished preprocessing" << std::endl;
    return true;
}

void Solver::assign(Literal_t lit) {
    assignments[var_index(lit)] = lsign(lit);
    trail.push_back(lit);
    decision_levels[var_index(lit)] = current_decision_level();
}

void Solver::pure_literal_elimination() {
    std::vector<bool> literal_ocurrence(watch_lists.size(), false);
    for (const auto& clause: clauses) {
        for (auto literal: clause->literals) {
            literal_ocurrence[literal] = true;
        }
    }
    for (int i = 0; i < literal_ocurrence.size(); ++i) {
        if (!literal_ocurrence[i]) {
            if (value(i) == UNASSIGNED) {
                assign(negate_literal(i));
            }
        }
    }
    // erase clauses if they contain a true literal
    auto deleted_clause = std::erase_if(clauses, [this](auto clause) {
        return contains_true_literal(clause);
    });
    std::cout << "Deleted " << deleted_clause << " clauses during pure literal elimination" << std::endl;
}

bool Solver::contains_true_literal(const std::shared_ptr<Clause>& clause) {
    return std::ranges::any_of(clause->literals, [this](Literal_t literal) {
        return value(literal) == TRUE;
    });
}

bool Solver::maybe_eliminate(Variable_t var, bool &out_eliminated_var, uint bound_factor) {
    uint max_resolvents = clauses.size() * bound_factor;
    auto positive_occurrences = occurrence_lists[positive_literal(var)];
    auto negative_occurrences = occurrence_lists[negative_literal(var)];
    return maybe_eliminate(positive_occurrences, negative_occurrences, out_eliminated_var, max_resolvents);
}

std::vector<Literal_t> Solver::resolve(const Clause_ptr &a, const Clause_ptr& b, Variable_t var) {
    std::set<Literal_t> resolvent;
    Clause_ptr input_clause = a;
    for (int n = 0; n < 2; ++n) {
        for (auto literal: input_clause->literals) {
            if (var_index(literal) == var) {
                // variables to be eliminated
                continue;
            }
            if (resolvent.contains(negate_literal(literal))) {
                // resolvent already contains the negated literal: Tautology
                return {};
            }
            resolvent.insert(literal);
        }
        input_clause = b;
    }
    std::vector<Literal_t> literals = {resolvent.begin(), resolvent.end()};
    return literals;
}

void Solver::delete_occurrence_lists() {
    for (auto list: occurrence_lists) {
        occurrence_lists.clear();
    }
}

bool
Solver::maybe_eliminate(std::vector<Clause_wptr> &positive_occurrence, std::vector<Clause_wptr> &negative_occurrence,
                        bool &out_eliminated_var, uint max_resolvents) {
    std::vector<std::vector<Literal_t>> resolvents;
    for (auto clause_p: positive_occurrence) {
        if (clause_p.lock()) {}
        for (auto clause_n: negative_occurrence) {

        }
    }
    return false;
}
