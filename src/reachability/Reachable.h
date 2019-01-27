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

        }



        BDD_ID xnor2(BDD_ID a, BDD_ID b);

        ~Reachable() ;
    };
//}

#endif //VDSPROJECT_IMGCOMP_H
