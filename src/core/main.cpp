#include "MyaiController.h"
#include <iostream>



int main(int argc, const char **argv) {
	MYAI_SPACE::MyaiController controller(10);
	controller.init();
	controller.run();
	std::cout << "Hello world!" << std::endl;
	return 0;
}