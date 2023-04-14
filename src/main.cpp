
#include <iostream>
#include <map>
#include <thread>
#include <unistd.h>

[[noreturn]] void tr(){
label:

sleep(1);


  goto label;
}

int main() {
std::thread t(tr);
std::thread t2(tr);
std::thread t3(tr);
std::thread t4(tr);
t.join();
t2.join();
t3.join();
t4.join();
}