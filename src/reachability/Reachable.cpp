//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"


BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b){
    BDD_ID negB =this->neg(b);

    return this->ite(a,b,negB);
}

const std::vector<BDD_ID> &Reachable::getStates() const{
    return *states;
}

void Reachable::initStates() {
    // one vector for positiv literals and one for negativ
    std::vector<BDD_ID>variablesPos=std::vector<BDD_ID>();
    std::vector<BDD_ID>variablesNeg=std::vector<BDD_ID>();
    std::vector<BDD_ID>* allStates = new std::vector<BDD_ID>();

    std::string label;

    for (int i = 0; i < state_var; ++i) {
        BDD_ID tmp= this->createVar("s"+std::to_string(i));
        variablesPos.push_back(tmp);
    }

    for (int i = 0; i < state_var; ++i) {
        BDD_ID notTmp=this->neg(variablesPos.at(i));
        variablesNeg.push_back(notTmp);
    }


    //printUniqueTable();

    bool  counter[state_var];
    bool finishedCounting =false;
    BDD_ID tmp;

    //idea counting up binary numbers form 0 to 2^state_var-1 and
    for (int i=0; i< state_var; i++){
        counter[i]= false;

        if (i==0){
            tmp=variablesNeg.at(0);
        }else {
            tmp = and2(tmp, variablesNeg.at(i));
        }
    }

    allStates->push_back(tmp);

    do{
        counter[0]=!counter[0];
        tmp = counter[0]?variablesPos.at(0):variablesNeg.at(0);
        // if the last bit is a 1 then it was step from 0 -> 1 so the current bit can't flip
        int j;
        for ( j = 1; j <  state_var && !counter[j-1]; ++j) {
            counter[j]=!counter[j];
            tmp = counter[j]?and2(tmp,variablesPos.at(j)):and2(tmp,variablesNeg.at(j));
        }
        // assign the rest of the bits unchanged for j...statevar-1;
        for ( int i = j; i <  state_var ; ++i) {
            tmp = counter[i]?and2(tmp,variablesPos.at(i)):and2(tmp,variablesNeg.at(i));
        }

        allStates->push_back(tmp);

        //stop tht do while loop when counter hits 11...1
        finishedCounting= true;
        for (int i=0; i< state_var && finishedCounting; i++){
            finishedCounting= counter[i]? true: false;
        }
    } while(!finishedCounting);

    states = allStates;
}

void Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions){
    if (transitionFunctions.size() == state_var){
        *transitons=transitionFunctions;
    }else{
        throw std::invalid_argument("too few or too many transitions for the amout of state variables");
    }

}

void Reachable::setInitState(const std::vector<bool>& stateVector){
    if (stateVector.size() == state_var){
        BDD_ID tmp;
        for (int i = 0; i < state_var; ++i) {
            if( i==0 ){
                if (stateVector.at(0)){
                    tmp = 2 + state_var-1;
                }else{
                    tmp = 2 + 2*state_var-1;
                }
            }else{
                if (stateVector.at(i)) {
                    tmp = and2(tmp, 2 + state_var - 1 - i);
                }else {
                    tmp = and2(tmp, 2 + 2*state_var - 1 - i);
                }
            }

        }
        initialstate=tmp;
    }else{
        throw std::invalid_argument("too few or too many state vaiable in this initial state");
    }

}

BDD_ID Reachable::compute_reachable_states(){


    return 0;
}