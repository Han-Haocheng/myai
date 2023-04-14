<<<<<<< HEAD

#include <string>
#include <vector>

#include "core/ThinkControl.h"

class Book
{
  int dakaideyeshu = 0;
  int zongyeshu    = 1000;

public:
  Book(int a = 0) : dakaideyeshu(a){};

  int f(int x, int y) { return x * x + y * y; }

  void dakai(int dakaidaoduoshaoye)
  {
    if (dakaidaoduoshaoye >= 0 && dakaidaoduoshaoye <= zongyeshu) { dakaideyeshu = dakaidaoduoshaoye; }
    throw "";
  }
};

int main() { printf("%d", static_cast<unsigned int>(-1)); }
=======
#include <iostream>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char **argv) {
	std::cout << "Hello " << std::endl;
	return 0;
}
>>>>>>> f215332 (2025-1-1)
