//
// Created by tobias on 29.11.18.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <iostream>
#include "Reachable.h"
#include "Manager.h"
#include "gtest/gtest.h"

using namespace ClassProject;

    TEST(xnor2, Equvalenz) {
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

/// Returns state amount
TEST(Reachable, getStates)
{
    uint stateVars = 3;
    Reachable r = Reachable(stateVars);
    // Size will produce stateVars * 2, this is due to adding the next state variables
    ASSERT_EQ(stateVars, r.getStates().size()-stateVars);
}
/// LSB and MSB test for getStates
TEST(Reachable, getStates_lsb_msb)
{
    Reachable r = Reachable(3); // NOTE: Constructor adds an additional 3 for next state variables
    const std::vector<BDD_ID> states = r.getStates();
    
    BDD_ID s0 = states.at(0);   // 2
    BDD_ID s1 = states.at(1);   // 3
    BDD_ID s2 = states.at(2);   // 4

    
    // Test initial without any arithmetic
    ASSERT_EQ(r.lsb, s0);
    ASSERT_EQ(r.msb, s2);
}
TEST(managerTest, HowTo_Example) {
    Reachable comp(2);

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
    // Add init state
    std::vector<bool> initialStateBits;
    initialStateBits.push_back(false);
    initialStateBits.push_back(false);
    comp.setInitState(initialStateBits);

    ASSERT_TRUE(comp.is_reachable({true,true}));
    ASSERT_TRUE(comp.is_reachable({false,false}));
    ASSERT_FALSE(comp.is_reachable({true,false}));
    ASSERT_FALSE(comp.is_reachable({false,true}));
}
#endif //VDSPROJECT_TESTS_H
