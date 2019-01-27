//
// Created by tobias on 29.11.18.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <iostream>
#include "Reachable.h"
#include "Manager.h"
#include "gtest/gtest.h"

namespace ClassProject {

    TEST(xnor2, Equvalenz) {
        //Manager m;
        Reachable r = Reachable(1);

        BDD_ID a = r.createVar("a");
        BDD_ID notA = r.neg(a);

        ASSERT_TRUE(r.xnor2(a, a));
        ASSERT_FALSE(r.xnor2(a,notA));
    }

    TEST(xnor2, NegationToXOR) {
        //Manager m;
        Reachable r = Reachable(1);

        BDD_ID a = r.createVar("a");
        BDD_ID b = r.createVar("b");
        BDD_ID c = r.createVar("c");

        BDD_ID notA = r.neg(a);
        BDD_ID AandB = r.and2(a,b);
        BDD_ID notAorC = r.and2(notA,c);

        ASSERT_EQ(r.xnor2(AandB, notAorC),r.neg(r.xor2(AandB, notAorC)));
    }

    TEST(getStates, amount){
        Reachable r =Reachable(3);

        ASSERT_EQ(8,r.getStates().size());
    }

    TEST(getStates, lsb_msb){
        Reachable r =Reachable(3);

        const std::vector<BDD_ID> &states = r.getStates();

        BDD_ID lsb,msb;

        if(states.size()>0){
            std::cout<<"hold"<<std::endl;
            lsb=states.at(0);
            msb=states.at(states.size()-1);
        }else{
            lsb=0;
            msb=0;
        }

        ASSERT_EQ(r.False(),r.and2(r.True(),lsb));
        // msb is s0=1,s1=1,s2=1
        ASSERT_EQ(r.True(),r.xor2(msb,lsb));
    }

/*TEST(managerTest, HowTo_Example) {

    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s0));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false});

    ASSERT_TRUE(comp.is_reachable({true,true}));
    ASSERT_TRUE(comp.is_reachable({false,false}));
    ASSERT_FALSE(comp.is_reachable({true,false}));
    ASSERT_FALSE(comp.is_reachable({false,true}));

}*/


}
#endif //VDSPROJECT_TESTS_H
