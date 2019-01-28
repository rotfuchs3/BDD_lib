#include <iostream>
#include "Manager.h"
#include "Reachable.h"
using namespace ClassProject;
int main (void)
{
  std::cout << "Hello from main" << std::endl;
  
  Reachable r(2);
  r.printUniqueTable();
  
  auto states = r.getStates();
  std::vector<BDD_ID> functions;
  
  auto s0 = states.at(0);
  auto s1 = states.at(1);
  
  // s0` = not(s0)
  functions.push_back(r.neg(s0));
  // s1` = not(s1)
  functions.push_back(r.neg(s1));
  
  r.setDelta(functions);
  r.printUniqueTable();
  
  // Now add inital state bits
  r.setInitState({false, false});
  return 0;
}
