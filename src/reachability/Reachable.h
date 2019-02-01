//
// Created by ludwig on 27.11.18.
//

#ifndef VDSPROJECT_IMGCOMP_H
#define VDSPROJECT_IMGCOMP_H


#include <map>
#include "ReachableInterface.h"


//namespace ClassProject {
class Reachable : public ClassProject::ReachableInterface {
    public:
        Reachable(unsigned int state_variable) : ReachableInterface(state_variable) {
            if (state_variable>0){
                state_var=state_variable;
                initStates();
            }else{
                throw std::invalid_argument("number of state variable should at least be 1");
            }

        }

        void setDelta(const std::vector<BDD_ID> &transitionFunctions);
        void setInitState(const std::vector<bool>& stateVector);
        BDD_ID xnor2(BDD_ID a, BDD_ID b);
        BDD_ID compute_reachable_states();

        const std::vector<BDD_ID> &getStates() const;

        ~Reachable() {}

    private:
        unsigned int state_var;
        std::vector<BDD_ID>* states;
        std::vector<BDD_ID>* transitons;

        BDD_ID initialstate;

        bool initaliseTransition = false;
        bool initaliseInitalState = false;

        // initialize the vector states for getStates()
        void initStates();

};


//}

#endif //VDSPROJECT_IMGCOMP_H
