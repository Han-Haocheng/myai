

#include <functional>
#include <iostream>

struct X {
  int x = 1;

  void f() { std::cout << "666"; }
  void z() { std::cout << "777"; }

  void z(int i) { std::cout << 777 + i; }
  void f(int i) { std::cout << i; }
};

struct Test_bind {
  void t(int n) {
    for (; n; n--) {
      std::cout << n << std::endl;
    }
  }
  void t() {
    std::cout << "t" << std::endl;
  }
};

void f2(void (X::*p)(int), X *x, int n) {
  (x->*p)(n);
}

void test1() {
  int X::*n = &X::x;
  X z;
  std::cout << z.x << "\n";
  int &v = (z.*n);
  v      = 100;
  std::cout << z.x << "\n";


  Test_bind tb;
  auto t = std::bind(static_cast<void (Test_bind::*)(int)>(&Test_bind::t), &tb, 2);
  t();
}

std::string operator""_r(const char *str, size_t size) {
  return {str};
}


void test2() {
  int c = 017;
  std::cout << "123455"_r<<__cplusplus
            << "\n";
  ;
}

int main() {
  test2();
}