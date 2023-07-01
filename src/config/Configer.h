//
// Created by HanHaocheng on 2023/11/7.
//

#ifndef MY_AI_CONFIGER_H
#define MY_AI_CONFIGER_H

#include "Config.h"
#include <map>
#include <string>
#include <utility>

namespace myai
{
class Configer
{
public:
  typedef std::shared_ptr<Configer> ptr;
  using YamlPair = std::pair<std::string, YAML::Node>;
  typedef std::unordered_map<std::string, ConfigBase::ptr> ConfigMap;

  void loadByYaml(const std::string &path);
  //void saveToYaml(const std::string &path);

  template<typename ConfTy>
  bool addConfig(const std::string &name, const ConfTy &defConf, const std::string &comment);
  bool delConfig(const std::string &name);
  template<typename ConfTy>
  typename Config<ConfTy>::ptr getConfig(const std::string &name);

  template<typename ConfTy>
  bool setConfig(const std::string &name, const ConfTy &val, const std::string &comment);

  void setAllDefault();

private:
  static void __ListAllYamlNode(YAML::Node &root_node, std::list<YamlPair> &out);
  ConfigBase::ptr _getConfigBase(const std::string &name);

private:
  ConfigMap m_configs;
};

template<typename ConfTy>
typename Config<ConfTy>::ptr Configer::getConfig(const std::string &name)
{
  auto fd_rt = m_configs.find(name);
  if (fd_rt == m_configs.end()) {
    std::cout << "get config faild,"
              << " config name=" << name
              << " is not exist." << std::endl;
    return nullptr;
  }
  if (fd_rt->second->getTypeName() != typeid(ConfTy).name()) {
    std::cout << "get config faild,config type is diffetent,"
              << "name=" << name << " "
              << "in type=" << typeid(ConfTy).name() << " "
              << "real type=" << fd_rt->second->getTypeName() << std::endl;
    return nullptr;
  }
  return std::dynamic_pointer_cast<Config<ConfTy>>(fd_rt->second);
}

template<typename ConfTy>
bool Configer::setConfig(const std::string &name, const ConfTy &val, const std::string &comment)
{
  auto fd_rt = getConfig<ConfTy>(name);
  if (!fd_rt) {
    std::cout << "set config faild,config is not exist."
              << "name=" << name << std::endl;
    return false;
  }
  fd_rt->setValue(val);
  return true;
}

template<typename ConfTy>
bool Configer::addConfig(const std::string &name, const ConfTy &defConf, const std::string &comment)
{
  auto fd_rt = getConfig<ConfTy>(name);
  if (fd_rt) {
    std::cout << "add config faild,config is exist."
              << "name=" << name << std::endl;
    return false;
  }
  m_configs.emplace(name, new Config<ConfTy>{name, defConf, comment});
  return true;
}
}// namespace myai

#endif//MY_AI_CONFIGER_H
