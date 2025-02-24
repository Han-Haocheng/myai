<<<<<<< HEAD
<<<<<<< HEAD
#include "MyaiController.h"
#include <iostream>



int main(int argc, const char **argv) {
	MYAI_SPACE::MyaiController controller(10);
	controller.init();
	controller.run();
	std::cout << "Hello world!" << std::endl;
	return 0;
}
=======
<<<<<<< HEAD
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
=======
#include <iostream>
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
#include "MyaiController.h"
#include <iostream>


<<<<<<< HEAD
int main(int argc, const char** argv) {
    MYAI_SPACE::MyaiController controller(10);
    controller.run();
    std::cout << "Hello world!" << std::endl;
    return 0;
}
>>>>>>> e675a70 (2025年2月12日 15:19:07)
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======

int main(int argc, const char **argv) {
	MYAI_SPACE::MyaiController controller(10);
	controller.init();
	controller.run();
	std::cout << "Hello world!" << std::endl;
	return 0;
}
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
