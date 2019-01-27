//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"





Reachable::BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b){
    BDD_ID negB =this->neg(b);
    
    return this->ite(a,b,negB);
}
