//
// Created by HanHaocheng on 2023/11/7.
//

#include "Configer.h"
#include "../log/LoggerManager.h"
#include <iostream>
#include <iterator>
#include <stack>
#include <utility>
namespace myai
{
Configer::ptr Configer::GetInstance()
{
  static Configer::ptr configer{new Configer{}};
  if (!configer) {
    configer.reset(new Configer);
  }
  return configer;
}

void Configer::list_all_yaml_node(YAML::Node &root_node, std::list<YamlPair> &out)
{
  std::stack<YamlPair> node_stack;
  node_stack.emplace("", root_node);
  while (!node_stack.empty()) {
    // 获取顶部节点
    auto topNode = node_stack.top();
    node_stack.pop();
    // 判断顶部节点类型 可能为 空 标量 列表 图
    // 只判断是否是图
    if (topNode.second.IsMap()) {
      for (const auto &node: topNode.second) {
        std::string key = topNode.first.empty() ? node.first.Scalar() : topNode.first + "." + node.first.Scalar();
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        if (key.find_first_not_of("abcdefgihgklmnopqrstuvwxyz.0123456789_") != std::string::npos) {
          std::cout << "node key=`" << key << "`is not a legal key. first faild char=" << key.find_first_not_of("abcdefgihgklmnopqrstuvwxyz.0123456789_") << std::endl;
        }
        out.emplace_back(key, node.second);
        node_stack.emplace(key, node.second);
      }
    }
  }
}

void Configer::loadByYaml(const std::string &path)
{
  YAML::Node root_nd;
  try {
    root_nd = YAML::LoadFile(path);
  } catch (YAML::BadFile &bf) {
    std::cout << "<warning Configer::loadByYaml>load yaml path=`" << path << "`is not exist." << std::endl;
  }

  std::list<YamlPair> all_nodes;
  list_all_yaml_node(root_nd, all_nodes);
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

Configer::Configer() = default;

}// namespace myai