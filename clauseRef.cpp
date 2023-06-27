//
// Created by benjamin on 27.06.23.
//

#include "clauseRef.h"

Clause ClauseRef::get() {
    return ref.get();
}

ClauseRef::ClauseRef(std::reference_wrapper<Clause> ref) : ref(ref)  {}
