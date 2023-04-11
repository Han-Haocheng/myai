
#include <iostream>
#include <map>


int main() {
  std::map<int,int>test{{1,1},{2,2},{3,3}};
 
  std::cout<< test.erase(1)<<std::endl;
  return 0;
}