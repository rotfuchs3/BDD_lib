#include <iostream>
#include "Manager.h"

int main (void)
{
  // Creating some dependency here, hopefully compiler doesn't optimize out. Testing cmake
  Manager *m = new Manager();
  std::cout << "Hello from main" << std::endl;
  Node value1=Node(2,3,4,2,"a");
  m->insert(2,value1);
  std::cout << "New Size: "<< m->size()<<std::endl;

  return 0;
}
