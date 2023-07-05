//
// Created by benjamin on 13.06.23.
//

#include <iostream>
#include <functional>
#include <cassert>
#include "solver.h"
#include "clause.h"
#include "encoding_util.h"


void Solver::addClause(const std::vector<Literal_t> &clause, bool learnt) {
    if (clause.empty()) {
        std::cout << "WARNING: Tried to add empty clause" << std::endl;
    } else if (clause.size() == 1) {
        enqueue(clause.back());
    } else {
        // select vector to append to
        std::vector<Clause> &clause_vector = learnt ? learnt_clauses : clauses;
        clause_vector.emplace_back(clause, learnt);
        // Set BaseAddress for debugging purposes
        if (clauses.size() == 1) {
            ClauseRef::setClausesBaseAddress(&clauses[0]);
        }
        if (learnt_clauses.size() == 1) {
            ClauseRef::setLearntClausesBaseAddress(&learnt_clauses[0]);
        }
        // Add clause to the watchlist of the negation of the two first elements
        watch_lists[negate_literal(clause[0])].emplace_back(std::ref(clause_vector.back()));
        watch_lists[negate_literal(clause[1])].emplace_back(std::ref(clause_vector.back()));

    }
}

bool Solver::enqueue(Literal_t literal, std::optional<ClauseRef> reason) {
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
        assignments[var_index(literal)] = lsign(literal);
        trail.push_back(literal);
        decision_levels[var_index(literal)] = current_decision_level();
        antecedent_clauses[var_index(literal)] = reason;
        propagation_queue.push(literal);
        return true;
    }
}

lbool Solver::value(Literal_t l) {
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
    assignments.reserve(number);
    decision_levels.reserve(number);
    antecedent_clauses.reserve(number);
    watch_lists.reserve(2*number);
    for (; number > 0; --number) {
        assignments.push_back(UNASSIGNED);
        decision_levels.push_back(DECISION_LEVEL_UNASSIGNED);
        antecedent_clauses.emplace_back();
        watch_lists.emplace_back();
        watch_lists.emplace_back();
    }

}

std::optional<ClauseRef> Solver::propagate() {
    while (!propagation_queue.empty()) {
        Literal_t literal = propagation_queue.front();
        std::cout << "LEVEL " << current_decision_level() << ": Propagating " << dimacs_format(literal) << std::endl;
        propagation_queue.pop();
        std::vector<ClauseRef> tmp_watchlist;
        tmp_watchlist = watch_lists[literal];
        watch_lists[literal].clear();
        //std::move(watch_lists[literal].begin(), watch_lists[literal].end(), tmp_watchlist.begin());
        for (int i = 0; i < tmp_watchlist.size(); ++i) {
            if(!tmp_watchlist[i].get().propagate(*this, literal)) {
                // Conflict occurred
                // reinsert remaining entries from tmp_watchlist
                for (int j = i + 1; j < tmp_watchlist.size(); ++j) {
                    watch_lists[literal].push_back(tmp_watchlist[j]);
                }
                return tmp_watchlist[i];
            }
        }

    }
    return std::nullopt;
}
/**
 * Should only be called once for every solver object, otherwise not enough space is reserved
 * @param clauses
 */
void Solver::addClauses(const std::vector<std::vector<int>>& input_clauses) {
    clauses.reserve(input_clauses.size());
    learnt_clauses.reserve(input_clauses.size()*10);
    for (const auto& literal_list: input_clauses) {
        addClause(internal_representation(literal_list), false);
    }
}

int Solver::current_decision_level() const {
    return trail_limits.size();
}

bool Solver::assume(Literal_t literal) {
    trail_limits.push_back(trail.size());
    return enqueue(literal);
}

// Warning: changes the content of conflicting clause
void Solver::analyse_conflict(ClauseRef &conflicting_clause, std::vector<Literal_t> &out_learnt, int& out_bt_level) {
    std::vector<bool> seen(assignments.size(), false);
    int counter = 0;
    std::optional<Literal_t> p = std::nullopt;
    std::vector<Literal_t> p_reason;

    // reserve the first entry for literal of current decision level.
    out_learnt.push_back(UINT32_MAX);
    out_bt_level = 0;
    do {
        p_reason.clear();
        conflicting_clause.get().calc_reason(*this, p, p_reason);
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
                conflicting_clause = antecedent_clause.value();
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

void Solver::record_learnt_clause(const std::vector<Literal_t>& clause) {
    if (learnt_clauses.size() == learnt_clauses.capacity()) {
        std::cout << "WARNING: Capacity limit for storing learnt clause reached, will not store further clauses";
        return;
    }
    if (clause.size() > 1) {
        addClause(clause, true);
        enqueue(clause[0], std::ref(learnt_clauses.back()));
    }
    enqueue(clause[0]);
}

bool Solver::solve() {
    lbool status = UNASSIGNED;
    while (status == UNASSIGNED) {
        status = search();
    }
    return status == TRUE;
}

lbool Solver::search() {
    while (true) {
        auto conflicting_clause = propagate();
        if (conflicting_clause) {
            // Conflict handling
            std::cout << "Conflict in " << conflicting_clause.value() << std::endl;
            if (current_decision_level() == 0) {
                return FALSE;
            }
            std::vector<Literal_t> learnt_clause;
            int backtrack_level = 0;
            analyse_conflict(conflicting_clause.value(), learnt_clause, backtrack_level);
            std::cout << "Backtrack to level " << backtrack_level << std::endl;
            backtrack_until(backtrack_level);
            std::cout << "Learnt clause: " << learnt_clause << std::endl;
            record_learnt_clause(learnt_clause);
        } else {
            // No conflict
            if (trail.size() == assignments.size()) {
                // All variables are assigned
                std::cout << "Satisfying assignment found: " << trail << std::endl;
                return TRUE;
            } else {
                // Take new assumption
                Literal_t next_assumption = next_unassigned_variable();
                std::cout << "LEVEL " << current_decision_level() + 1 << ": Assuming " << dimacs_format(next_assumption) << std::endl;
                assume(next_assumption);
            }
        }
    }
}

Literal_t Solver::next_unassigned_variable() {
    for (int i = 0; i < assignments.size(); ++i) {
        if (assignments[i] == UNASSIGNED) {
            return i << 1;
        }
    }
}

void Solver::print_clauses() {
    for (const auto& clause: clauses) {
        std::cout << clause << std::endl;
    }
    if (!learnt_clauses.empty()) {
        std::cout << "Learnt Clauses:" << std::endl;
        for (const auto& clause: learnt_clauses) {
            std::cout << clause << std::endl;
        }
    }
}
