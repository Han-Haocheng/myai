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
  typename Config<ConfTy>::ptr setConfig(const std::string &name, const ConfTy &val, const std::string &comment)
  {
    try {
      auto fd_rt = getConfig<ConfTy>(name);
      if (!fd_rt) {
        fd_rt = std::make_shared<Config<ConfTy>>(name, val, comment);
        m_configs.emplace(name, fd_rt);
      }

      fd_rt->setValue(val);
      return fd_rt;
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    }
    return nullptr;
  }

  template<typename ConfTy>
  typename Config<ConfTy>::ptr getConfig(const std::string &name)
  {
    auto fd_rt = m_configs.find(name);
    if (fd_rt == m_configs.end()) {
      return nullptr;
    }
    if (fd_rt->second->getTypeName() != typeid(ConfTy).name()) {
      throw std::logic_error{(std::stringstream{"<error Configer::getConfig>"}
                              << "get config faild,config type is diffetent,"
                              << "name=" << name << " "
                              << "in type=" << typeid(ConfTy).name() << " "
                              << "real type=" << fd_rt->second->getTypeName())
                                 .str()};
    }
    return std::dynamic_pointer_cast<Config<ConfTy>>(fd_rt->second);
  }

  bool delConfig(const std::string &name);

  static Configer::ptr GetInstance();

private:
  Configer();
  static void list_all_yaml_node(YAML::Node &root_node, std::list<YamlPair> &out);
  ConfigBase::ptr _getConfigBase(const std::string &name);

private:
  ConfigMap m_configs;
  std::vector<std::function<void(ConfigBase::ptr, ConfigBase::ptr &)>> m_cbs;
};

}// namespace myai

#endif//MY_AI_CONFIGER_H
