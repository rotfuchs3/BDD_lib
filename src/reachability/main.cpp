#include <iostream>
#include "Manager.h"
#include "Reachable.h"
using namespace ClassProject;
#define ASSERT_EQ(eq, what) \
    std::cout << std::to_string(eq) << " == " << std::to_string(what) << std::endl
int main (void)
{ 
    uint state_var = 2;
    Reachable comp(state_var);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    
    if(state_var == 3)
    {
        auto s2 = states.at(2);
        functions.push_back(comp.neg(s2));
    }
    //Add transition functions
    comp.setDelta(functions);
    // Add init state
    std::vector<bool> initialStateBits;
    initialStateBits.push_back(false);
    initialStateBits.push_back(false);
    if(state_var == 3)
    {
        initialStateBits.push_back(false);
    }
    comp.setInitState(initialStateBits);
    comp.compute_reachable_states();
    comp.printVectors();
    //comp.printTables();
    
    ASSERT_EQ(1, comp.is_reachable({true,true}));
    ASSERT_EQ(1, comp.is_reachable({false,false}));
    ASSERT_EQ(0, comp.is_reachable({true,false}));
    ASSERT_EQ(0, comp.is_reachable({false,true}));
  return 0;
}
