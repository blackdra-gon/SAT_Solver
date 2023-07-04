//
// Created by benjamin on 27.06.23.
//

#include "clauseRef.h"

Clause* ClauseRef::clauses_base_address = std::nullptr_t();
Clause* ClauseRef::learnt_clauses_base_address = std::nullptr_t();

Clause& ClauseRef::get() const{
    return ref.get();
}

ClauseRef::ClauseRef(std::reference_wrapper<Clause> ref) : ref(ref)  {
    auto address = &get();
    learnt = get().learnt;
    Clause* base_address = learnt ? ClauseRef::getLearntClausesBaseAddress() : ClauseRef::getClausesBaseAddress();
    auto offset = address - base_address;
    index = offset;
}

bool ClauseRef::operator==(const Clause& other) const {
    return get() == other;
}

void ClauseRef::setClausesBaseAddress(Clause *clausesBaseAddress) {
    clauses_base_address = clausesBaseAddress;
}

std::ostream &operator<<(std::ostream &os, const ClauseRef &output) {
    if (output.learnt) {
        os << "Learnt_Clause[" << output.index << "]";
    } else {
        os << "Clause[" << output.index << "]";
    }
    return os;
}

Clause *ClauseRef::getClausesBaseAddress() {
    return clauses_base_address;
}

void ClauseRef::setLearntClausesBaseAddress(Clause *learntClausesBaseAddress) {
    learnt_clauses_base_address = learntClausesBaseAddress;
}

Clause *ClauseRef::getLearntClausesBaseAddress() {
    return learnt_clauses_base_address;
}

