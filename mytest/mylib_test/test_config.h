//
// Created by HanHaocheng on 2024/4/7.
//

#include "../../mylib/config/config_manager.h"
#include "../test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(config_suite_test)

BOOST_AUTO_TEST_CASE(test_config_base) {

  mylib::ConfigManager::ptr a{new mylib::ConfigManager};
  a->setConfigFile("config.yml");
  //auto res = std::make_shared<mylib::ConfigValue<std::vector<int>>>("key", std::vector<int>{123, 1234, 1234}, "std::move(comment)");
  a->setConfig<std::vector<int>>("config", {9, 0}, "config");
  a->setConfig<int>("config2", 9, "config2");
  a->setConfig<std::map<std::string, int>>("config3", {{"config4", 1}}, "config3");
  a->setConfig<int>("config3.config4", 9, "config2");
  a->setConfig<int>("config3.config5", 9, "config2");
  a->showAllConfigs();
  a->load();
  a->showAllConfigs();
}

void PrintNode(const YAML::Node &node, const std::string &prefix = "") {
  // 判断节点类型
  if (node.IsNull()) {
    std::cout << prefix << "null" << std::endl;
  } else if (node.IsScalar()) {
    std::cout << prefix << node.as<std::string>() << std::endl;
  } else if (node.IsMap()) {
    for (const auto &entry: node) {
      std::cout << prefix << "- Key: " << entry.first.as<std::string>() << std::endl;
      PrintNode(entry.second, prefix + "  ");// 递归打印子节点
    }
  } else if (node.IsSequence()) {
    for (size_t i = 0; i < node.size(); ++i) {
      std::cout << prefix << "- Item " << i << ":" << std::endl;
      PrintNode(node[i], prefix + "  ");// 递归打印子节点
    }
  }
}

BOOST_AUTO_TEST_CASE(test_yaml) {
  std::string yaml_str = R"(
root:
  scalar: Hello, World!
  sequence:
    - item1
    - item2
    - item3
  mapping:
    key1: value1
    key2: value2
)";

  YAML::Node root = YAML::Load(yaml_str);
  PrintNode(root);
}

BOOST_AUTO_TEST_SUITE_END()