//
// Created by HanHaocheng on 2023/11/7.
//

#ifndef MY_AI_CONFIG_H
#define MY_AI_CONFIG_H

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

namespace myai
{

template<class F, class T>
class ConfigCast
{
public:
  T operator()(const F &v) { return boost::lexical_cast<T>(v); }
};

template<typename T>
class ConfigCast<std::string, std::vector<T>>
{
public:
  std::vector<T> operator()(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::vector<T> vec;
    for (auto &&item_str: node)
      vec.emplace_back(ConfigCast<std::string, T>()(YAML::Dump(item_str)));
    return vec;
  }
};

template<typename T>
class ConfigCast<std::vector<T>, std::string>
{
public:
  std::string operator()(const std::vector<T> &v)
  {
    YAML::Node node;
    for (const auto &item: v)
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(item)));
    return YAML::Dump(node);
  }
};

template<typename T>
class ConfigCast<std::string, std::list<T>>
{
public:
  std::list<T> operator()(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::list<T> list;
    for (auto &&i: node)
      list.emplace_back(ConfigCast<std::string, T>()(YAML::Dump(i)));
    return list;
  }
};
template<typename T>
class ConfigCast<std::list<T>, std::string>
{
public:
  std::string operator()(const std::list<T> &v)
  {
    YAML::Node node;
    for (const auto &i: v)
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(i)));
    return YAML::Dump(node);
  }
};

template<typename T>
class ConfigCast<std::string, std::forward_list<T>>
{
public:
  std::forward_list<T> operator()(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::forward_list<T> fd_list;
    for (auto &&i: node)
      fd_list.emplace_back(ConfigCast<std::string, T>()(YAML::Dump(i)));
    return fd_list;
  }
};

template<typename T>
class ConfigCast<std::forward_list<T>, std::string>
{
public:
  std::string operator()(const std::forward_list<T> &v)
  {
    YAML::Node node;
    for (const auto &i: v)
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(i)));
    return YAML::Dump(node);
  }
};

template<typename T>
class ConfigCast<std::string, std::set<T>>
{
public:
  std::set<T> operator()(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::set<T> set;
    for (auto &&i: node)
      set.insert(ConfigCast<std::string, T>()(YAML::Dump(i)));
    return set;
  }
};
template<typename T>
class ConfigCast<std::set<T>, std::string>
{
public:
  std::string operator()(const std::set<T> &v)
  {
    YAML::Node node;
    for (const auto &i: v)
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(i)));
    return YAML::Dump(node);
  }
};
template<class T>
class ConfigCast<std::string, std::unordered_set<T>>
{
public:
  std::unordered_set<T> operator()(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::unordered_set<T> un_set;
    for (auto &&i: node)
      un_set.insert(ConfigCast<std::string, T>()(YAML::Dump(i)));
    return un_set;
  }
};

/// unordered_set -> string
template<class T>
class ConfigCast<std::unordered_set<T>, std::string>
{
public:
  std::string operator()(const std::unordered_set<T> &un_set)
  {
    YAML::Node node;
    for (const auto &i: un_set)
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(i)));
    return YAML::Dump(node);
  }
};

/// string -> map
template<class T>
class ConfigCast<std::string, std::map<std::string, T>>
{
public:
  std::map<std::string, T> operator()(const std::string &v)
  {
    YAML::Node nodes = YAML::Load(v);
    typename std::map<std::string, T> map;
    for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it)
      map.insert(std::make_pair(node_it->first.Scalar(),
                                ConfigCast<std::string, T>()(YAML::Dump(node_it->second))));
    return map;
  }
};

/// map -> string
template<class T>
class ConfigCast<std::map<std::string, T>, std::string>
{
public:
  std::string operator()(const std::map<std::string, T> &map)
  {
    YAML::Node node;
    for (auto &i: map)
      node[i.first] = YAML::Load(ConfigCast<T, std::string>()(i.second));
    return YAML::Dump(node);
  }
};

/// string -> unordered_map
template<class T>
class ConfigCast<std::string, std::unordered_map<std::string, T>>
{
public:
  std::unordered_map<std::string, T> operator()(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::unordered_map<std::string, T> un_map;
    for (auto it = node.begin(); it != node.end(); ++it)
      un_map.insert(std::make_pair(it->first.Scalar(),
                                   ConfigCast<std::string, T>()(YAML::Dump(it->second))));
    return un_map;
  }
};

/// unordered_map -> string
template<class T>
class ConfigCast<std::unordered_map<std::string, T>, std::string>
{
public:
  std::string operator()(const std::unordered_map<std::string, T> &map)
  {
    YAML::Node node;
    for (auto &i: map) {
      node[i.first] = YAML::Load(ConfigCast<T, std::string>()(i.second));
    }
    return YAML::Dump(node);
  }
};
//=====================================================================================================================

class ConfigBase
{
public:
  typedef std::shared_ptr<ConfigBase> ptr;
  ConfigBase(std::string name, std::string comment);
  virtual ~ConfigBase() = default;
  const std::string &getName() const;
  const std::string &getComment() const;

  virtual std::string getTypeName() const = 0;
  virtual std::string toString() const = 0;
  virtual void fromString(const std::string &str) = 0;

private:
  std::string m_name;
  std::string m_comment;
};
//using ConfTy = int;
template<typename ConfTy, typename ToStr = ConfigCast<ConfTy, std::string>, typename FromStr = ConfigCast<std::string, ConfTy>>
class Config : public ConfigBase
{
public:
  typedef std::shared_ptr<Config<ConfTy>> ptr;
  using on_lisen_cb = std::function<void(const ConfTy &, const ConfTy &)>;
  Config(const std::string &name, const ConfTy &confval, const std::string &comment)
      : ConfigBase(name, comment)
  {
    m_val.reset(new ConfTy{confval});
  }

  std::string getTypeName() const override { return typeid(*m_val).name(); }

  std::string toString() const override
  {
    try {
      return ToStr{}(*(m_val));
    } catch (std::exception &e) {
      std::cout << "type cast faild, "
                << "except=" << e.what() << std::endl;
    } catch (...) {
      std::cout << "type cast faild." << std::endl;
    }
    return "";
  }

  void fromString(const std::string &str) override
  {
    try {
      setValue(FromStr{}(str));
    } catch (std::exception &e) {
      std::cout << "type cast faild, "
                << "except=" << e.what() << std::endl;
    } catch (...) {
      std::cout << "type cast faild." << std::endl;
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

  const ConfTy &getValue() { return *(m_val); }

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
  std::shared_ptr<ConfTy> m_val = nullptr;
  std::vector<on_lisen_cb> m_cbs;
};
}// namespace myai

#endif//MY_AI_CONFIG_H
