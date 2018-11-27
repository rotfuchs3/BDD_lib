#include <iostream>
#include "Manager.h"
int main (void)
{
  // Creating some dependency here, hopefully compiler doesn't optimize out. Testing cmake
  Manager *m = new Manager();
  std::cout << "Hello from main" << std::endl;
  return 0;
}
