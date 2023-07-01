//
// Created by benjamin on 13.06.23.
//

#include <iostream>
#include <functional>
#include <cassert>
#include "solver.h"
#include "clause.h"
#include "encoding_util.h"


void Solver::addClause(const Clause& clause) {
    if (clause.literals.empty()) {
        std::cout << "WARNING: Tried to add empty clause" << std::endl;
    } else if (clause.literals.size() == 1) {
        enqueue(clause.literals.back());
    } else {
        clauses.push_back(clause);
        if (clauses.size() == 1) {
            ClauseRef::setClausesBaseAddress(&clauses[0]);
        }
        // Add clause to the watchlist of the negation of the two first elements
        watch_lists[negate_literal(clause.literals[0])].emplace_back(std::ref(clauses.back()));
        watch_lists[negate_literal(clause.literals[1])].emplace_back(std::ref(clauses.back()));
    }
}

bool Solver::enqueue(Literal_t literal, std::optional<std::reference_wrapper<Clause>> reason) {
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
    for (const auto& literal_list: input_clauses) {
        addClause(internal_representation(literal_list));
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

}

void Solver::backtrack_one_level() {
    int steps = trail.size() - trail_limits.back();
    for (; steps != 0)
}
