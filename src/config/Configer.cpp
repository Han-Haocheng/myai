//
// Created by HanHaocheng on 2023/11/7.
//

#include "Configer.h"
#include "Config.h"
#include <functional>
#include <iostream>
#include <iterator>
#include <stack>
#include <utility>
namespace myai
{

void Configer::__ListAllYamlNode(YAML::Node &root_node, std::list<YamlPair> &out)
{
  std::stack<YamlPair> node_stack;
  node_stack.push({"", root_node});
  while (!node_stack.empty()) {
    YamlPair top = node_stack.top();
    node_stack.pop();
    std::transform(top.first.begin(), top.first.end(), top.first.begin(), ::tolower);
    if (top.first.find_first_not_of("abcdefghijklmnopqrstuvwxyz_0123456789.") != std::string::npos) {
      std::cout << "node name is not valid, "
                << "name=" << top.first << std::endl;
      throw std::runtime_error("node name is not valid");
    }

    out.emplace_back(top);
    for (const auto &item: top.second) {
      if (item.IsMap()) {
        node_stack.push({
            top.first.empty()
                ? item.first.Scalar()
                : top.first + '.' + item.first.as<std::string>(),
            item.second,
        });
      }
    }
  }
}

void Configer::loadByYaml(const std::string &path)
{
  YAML::Node root_nd = YAML::LoadFile(path);
  std::list<YamlPair> all_nodes;
  __ListAllYamlNode(root_nd, all_nodes);
  for (const auto &nd_pair: all_nodes) {
    if (nd_pair.first.empty()) {
      continue;
    }
    ConfigBase::ptr var = _getConfigBase(nd_pair.first);

    if (!var) {
      return;
    }

    var->fromString(nd_pair.second.IsScalar()
                        ? nd_pair.second.Scalar()
                        : YAML::Dump(nd_pair.second));
  }
}

ConfigBase::ptr Configer::_getConfigBase(const std::string &name)
{
  auto fd_rt = m_configs.find(name);
  if (fd_rt == m_configs.end()) {
    std::cout << "get config faild,"
              << " config name=" << name
              << " is not exist." << std::endl;
    return nullptr;
  }
  return fd_rt->second;
}



bool Configer::delConfig(const std::string &name)
{
  auto fd_rt = getConfig<ConfigBase>(name);
  if (!fd_rt) {
    std::cout << "del config faild,config is not exist."
              << "name=" << name << std::endl;
    return false;
  }
  m_configs.erase(name);
  return false;
}

void Configer::setAllDefault()
{
  for (const auto &config: m_configs) {
    config.second->setDefault();
  }
}

}// namespace myai