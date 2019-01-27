//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"


BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b){
    BDD_ID negB =this->neg(b);

    return this->ite(a,b,negB);
}

const std::vector<BDD_ID> &Reachable::getStates() const{
    const std::vector<BDD_ID>&states = std::vector<BDD_ID>(0);
    return states;
}