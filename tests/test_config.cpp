//
// Created by HanHaocheng on 2023/11/18.
//
#include "../src/config/Configer.h"

int main(int argc, const char *argv[])
{
  myai::Configer testConfiger = myai::Configer();
  testConfiger.setConfig("tests", 123, "test");
  std::cout << testConfiger.getConfig<int>("tests")->getValue() << std::endl;
  testConfiger.loadByYaml("tests.yml");
  std::cout << testConfiger.getConfig<int>("tests")->getValue() << std::endl;

  return 0;
}