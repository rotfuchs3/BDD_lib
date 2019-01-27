#include <iostream>
#include "Manager.h"
#include "Reachable.h"
using namespace ClassProject;
int main (void)
{
  // Creating some dependency here, hopefully compiler doesn't optimize out. Testing cmake
  size_t state_var=0;
  //Reachable *r = new Reachable(0);
  std::cout << "Hello from main" << std::endl;
  return 0;
}
