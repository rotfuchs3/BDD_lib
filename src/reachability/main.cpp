#include <iostream>
#include "Manager.h"
#include "Reachable.h"
#define ASSERT_EQ(eq, what) \
    std::cout << std::to_string(eq) << " == " << std::to_string(what) << std::endl
using namespace ClassProject;
int main (void)
{
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

    ASSERT_EQ(1, comp.is_reachable({true,true}));
    ASSERT_EQ(1, comp.is_reachable({false,false}));
    ASSERT_EQ(0, comp.is_reachable({true,false}));
    ASSERT_EQ(0, comp.is_reachable({false,true}));
  return 0;
}
