//
// Created by HanHaocheng on 2023/12/4.
//

#include <iostream>
#include <unordered_map>

void test1()
{
  clock_t start, ends;
  std::unordered_map<int, int> map1;
  for (int i = 0; i < (100000); ++i) {
    map1.emplace(i, i);
  }
  std::cout << "default\tmap1 size=" << map1.size();
  std::unordered_map<int, int> map2;
  std::cout << "\tmap2 size=" << map2.size() << std::endl;

  start = clock();
  map2 = std::move(map1);
  ends = clock();

  std::cout << "after"
            << "\tmap1 size=" << map1.size()
            << "\tmap2 size=" << map2.size()
            << "\tuse time=" << ends - start << "ms" << std::endl;
}
enum Test
{
  A,
  B,
  C
};
struct TTT {
  Test t;
};
void test2()
{
  TTT t{};
  std::cout <<( t.t==Test::A?"A":"B | C") << std::endl;
}
int main(int argc, char **argv)
{

  test2();

  return 0;
}