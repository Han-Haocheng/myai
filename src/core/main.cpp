#include <iostream>
#include "MyaiController.h"


int main(int argc, const char** argv) {
    MYAI_SPACE::MyaiController controller(10);
    controller.run();
    std::cout << "Hello world!" << std::endl;
    return 0;
}