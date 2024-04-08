//
// Created by HanHaocheng on 2024/4/7.
//

#define BOOST_TEST_MODULE mylib_test_suite

#include <boost/test/unit_test.hpp>
#include <yaml-cpp/yaml.h>

#include "../../mylib/config/config_manager.h"

BOOST_AUTO_TEST_SUITE(suite_test)

BOOST_AUTO_TEST_CASE(test_config_base) {
  mylib::test();
  mylib::ConfigManager::ptr a{new mylib::ConfigManager};
  a->setConfigFile("asdf");
  auto res = std::make_shared<mylib::ConfigValue<std::vector<int>>>("key", std::vector<int>{123, 1234, 1234}, "std::move(comment)");
  //a->setConfig<std::vector<int>>("config.vec", {9, 0}, "config vec");
  //a->showAllConfigs();
  //a->load();
}

BOOST_AUTO_TEST_CASE(test_yaml) {
  YAML::Node a = YAML::LoadFile("config.yml");
  a.begin();
  std::cout << YAML::Dump(a) << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()