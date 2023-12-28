//
// Created by HanHaocheng on 2023/11/18.
//
#include "../include/config.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(suite_config)

BOOST_AUTO_TEST_CASE(test_base)
{
  mylib::Configer::ptr testConfiger = mylib::Configer::GetInstance();
  testConfiger->setConfig("tests", 123, "test");
  std::cout << testConfiger->getConfig<int>("tests")->getValue() << std::endl;
  testConfiger->loadByYaml("tests.yml");
  std::cout << testConfiger->getConfig<int>("tests")->getValue() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
