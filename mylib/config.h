//
// Created by HanHaocheng on 2023/12/21.
//

#ifndef MY_LIB_CONFIG_H
#define MY_LIB_CONFIG_H

#include <boost/lexical_cast.hpp>
#include <cstddef>
#include <forward_list>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace mylib
{

//=====================================================================================================================

template<class T>
class Formatter
{
public:
  std::string toString(const T &val) { return boost::lexical_cast<std::string>(val); }
  T fromString(const std::string &str) { return boost::lexical_cast<T>(str); }
};
template<class T>
class Formatter<std::vector<T>>
{
public:
  std::string toString(const std::vector<T> &vec)
  {
    YAML::Node node;
    for (const auto &item: vec)
      node.push_back(YAML::Load(Formatter<T>().toString(item)));
    return YAML::Dump(node);
  }
  std::vector<T> fromString(const std::string &str)
  {
    YAML::Node node = YAML::Load(str);
    typename std::vector<T> vec;
    for (auto &&item_str: node)
      vec.emplace_back(Formatter<T>().fromString(YAML::Dump(item_str)));
    return vec;
  }
};
template<class T>
class Formatter<std::list<T>>
{
public:
  std::string toString(const std::list<T> &list)
  {
    YAML::Node node;
    for (const auto &i: list)
      node.push_back(YAML::Load(Formatter<T>().toString(i)));
    return YAML::Dump(node);
  }
  std::list<T> fromString(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::list<T> list;
    for (auto &&i: node)
      list.emplace_back(Formatter<T>{}.fromString(YAML::Dump(i)));
    return list;
  }
};

template<class T>
class Formatter<std::forward_list<T>>
{
public:
  std::string toString(const std::forward_list<T> &fwd_list)
  {
    YAML::Node node;
    for (const auto &i: fwd_list)
      node.push_back(YAML::Load(Formatter<T>().toString(i)));
    return YAML::Dump(node);
  }
  std::forward_list<T> fromString(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::forward_list<T> list;
    for (auto &&i: node)
      list.emplace_front(Formatter<T>().fromString(Dump(i)));
    return list;
  }
};

template<class T>
class Formatter<std::set<T>>
{
public:
  std::string toString(const std::set<T> &set)
  {
    YAML::Node node;
    for (const auto &i: set)
      node.push_back(YAML::Load(Formatter<T>().toString(i)));
    return YAML::Dump(node);
  }
  std::set<T> fromString(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::set<T> set;
    for (auto &&i: node)
      set.insert(Formatter<T>().fromString(YAML::Dump(i)));
    return set;
  }
};

template<class T>
class Formatter<std::unordered_set<T>>
{
public:
  std::string toString(const std::unordered_set<T> &un_set)
  {
    YAML::Node node;
    for (const auto &i: un_set)
      node.push_back(YAML::Load(Formatter<T>().toString(i)));
    return YAML::Dump(node);
  }
  std::unordered_set<T> fromString(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::unordered_set<T> un_set;
    for (auto &&i: node)
      un_set.insert(Formatter<T>().fromString(YAML::Dump(i)));
    return un_set;
  }
};

template<class T>
class Formatter<std::map<std::string, T>>
{
public:
  std::string toString(const std::map<std::string, T> &map)
  {
    YAML::Node node;
    for (auto &val: map)
      node[val.first] = YAML::Load(Formatter<T>().toString(val.second));
    return YAML::Dump(node);
  }
  std::map<std::string, T> fromString(const std::string &str)
  {
    YAML::Node nodes = YAML::Load(str);
    typename std::map<std::string, T> map;
    for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it)
      map.insert(std::make_pair(node_it->first.Scalar(),
                                Formatter<T>().fromString(YAML::Dump(node_it->second))));
    return map;
  }
};

template<class T>
class Formatter<std::unordered_map<std::string, T>>
{
public:
  std::string toString(const std::unordered_map<std::string, T> &un_map)
  {
    YAML::Node node;
    for (auto &i: un_map) {
      node[i.first] = YAML::Load(Formatter<T>().toString(i.second));
    }
    return YAML::Dump(node);
  }

  std::unordered_map<std::string, T> fromString(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::unordered_map<std::string, T> un_map;
    for (auto it = node.begin(); it != node.end(); ++it)
      un_map.insert(std::make_pair(it->first.Scalar(),
                                   Formatter<T>().fromString(YAML::Dump(it->second))));
    return un_map;
  }
};

//=====================================================================================================================

class ConfigBase
{
public:
  typedef std::shared_ptr<ConfigBase> ptr;
  ConfigBase(std::string name, std::string comment);
  virtual ~ConfigBase() = default;
  [[nodiscard]] const std::string &getName() const;
  [[nodiscard]] const std::string &getComment() const;

  [[nodiscard]] virtual std::string getTypeName() const = 0;
  [[nodiscard]] virtual std::string toString() const = 0;
  virtual void fromString(const std::string &str) = 0;

private:
  std::string m_name;
  std::string m_comment;
};

//=====================================================================================================================
//using ConfTy = int;
template<typename ConfTy, typename Formatter = Formatter<ConfTy>>
class Config : public ConfigBase
{
public:
  typedef std::shared_ptr<Config<ConfTy>> ptr;
  using on_lisen_cb = std::function<void(const ConfTy &, const ConfTy &)>;
  Config(const std::string &name, const ConfTy &confVal, const std::string &comment)
      : ConfigBase(name, comment), m_val(new ConfTy{confVal}) {}

  [[nodiscard]] std::string getTypeName() const override { return typeid(*m_val).name(); }

  [[nodiscard]] std::string toString() const override
  {
    try {
      if (!m_val) {
        return "";
      }
      return Formatter{}.toString(*m_val);
    } catch (std::exception &e) {
      std::cout << "type cast failed, "
                << "except=" << e.what() << std::endl;
    } catch (...) {
      std::cout << "type cast failed." << std::endl;
    }
    return "";
  }

  void fromString(const std::string &str) override
  {
    try {
      setValue(Formatter{}.fromString(str));
    } catch (std::exception &e) {
      std::cout << "type cast failed, "
                << "except=" << e.what() << std::endl;
    } catch (...) {
      std::cout << "type cast failed." << std::endl;
    }
  }

  void setValue(const ConfTy &val)
  {
    auto old = m_val;
    m_val.reset(new ConfTy{val});
    for (const auto &cb: m_cbs) {
      cb(*old, *m_val);
    }
  }
  //void setValue(ConfTy &&val) { m_val.reset(new ConfTy{std::move(val)}); }

  const ConfTy &getValue() { return *m_val; }

  size_t addListener(const on_lisen_cb &cb)
  {
    m_cbs.emplace_back(cb);
    return m_cbs.size() - 1;
  }

  bool delListener(size_t pos)
  {
    return m_cbs.erase(m_cbs.begin() + pos) > 0;
  }

private:
  std::shared_ptr<ConfTy> m_val = {new ConfTy{}};
  std::vector<on_lisen_cb> m_cbs;
};
//=====================================================================================================================
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
      typename Config<ConfTy>::ptr fd_rt = getConfig<ConfTy>(name);
      if (!fd_rt) {
        fd_rt.reset(new Config<ConfTy>{name, val, comment});
        m_configs.emplace(name, fd_rt);
      }

      fd_rt->setValue(val);
      return fd_rt;
    } catch (std::exception &e) {
      std::cout << "<error Configer::setConfig>" << e.what() << std::endl;
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
      std::cout << "<error Configer::getConfig>get config failed,config type is difference"
                << "name=" << name << " "
                << "in type=" << typeid(ConfTy).name() << " "
                << "real type=" << fd_rt->second->getTypeName() << std::endl;
      return nullptr;
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
}// namespace mylib

#endif//MY_LIB_CONFIG_H
