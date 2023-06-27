//
// Created by benjamin on 27.06.23.
//

#include "clauseRef.h"

Clause* ClauseRef::clauses_base_address = std::nullptr_t();

Clause& ClauseRef::get() const{
    return ref.get();
}

ClauseRef::ClauseRef(std::reference_wrapper<Clause> ref) : ref(ref)  {
    auto address = &get();
    auto offset = address - ClauseRef::getClausesBaseAddress();
    index = offset;
}

bool ClauseRef::operator==(const Clause& other) const {
    return get() == other;
}

void ClauseRef::setClausesBaseAddress(Clause *clausesBaseAddress) {
    clauses_base_address = clausesBaseAddress;
}

std::ostream &operator<<(std::ostream &os, const ClauseRef &output) {
    os << "Clause[" << output.index << "]";
    return os;
}

Clause *ClauseRef::getClausesBaseAddress() {
    return clauses_base_address;
}

