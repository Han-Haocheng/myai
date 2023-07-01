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

    std::stringstream ss;
    for (auto &&item_str: node) {
      ss.str("");
      ss << item_str;
      T t = ConfigCast<std::string, T>()(ss.str());
      vec.emplace_back(t);
    }
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
    for (const auto &item: v) {
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(item)));
    }
    std::stringstream ss;

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

    std::stringstream ss;
    for (auto &&i: node) {
      ss.str("");
      ss << i;
      T t = ConfigCast<std::string, T>()(ss.str());
      list.emplace_back(t);
    }
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
    for (const auto &i: v) {
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(i)));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
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

    std::stringstream ss;
    for (auto &&i: node) {
      ss.str("");
      ss << i;
      T t = ConfigCast<std::string, T>()(ss.str());
      fd_list.emplace_back(t);
    }
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
    for (const auto &i: v) {
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(i)));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
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

    std::stringstream ss;
    for (auto &&i: node) {
      ss.str("");
      ss << i;
      T t = ConfigCast<std::string, T>()(ss.str());
      set.insert(t);
    }
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
    for (const auto &i: v) {
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(i)));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
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
    std::stringstream ss;
    for (auto &&i: node) {
      ss.str("");
      ss << i;

      T t = ConfigCast<std::string, T>()(ss.str());
      un_set.insert(t);
    }
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
    for (const auto &i: un_set) {
      node.push_back(YAML::Load(ConfigCast<T, std::string>()(i)));
    }
    std::stringstream ss;
    ss << node;

    return ss.str();
  }
};

/// string -> map
template<class T>
class ConfigCast<std::string, std::map<std::string, T>>
{
public:
  std::map<std::string, T> operator()(const std::string &v)
  {
    YAML::Node node = YAML::Load(v);
    typename std::map<std::string, T> map;
    std::stringstream ss;
    for (auto it = node.begin(); it != node.end(); ++it) {
      ss.str("");
      ss << it->second;

      T t = ConfigCast<std::string, T>()(ss.str());
      map.insert(std::make_pair(it->first.Scalar(), t));
    }
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
    for (auto &i: map) {
      node[i.first] = YAML::Load(ConfigCast<T, std::string>()(i.second));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
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
    std::stringstream ss;
    for (auto it = node.begin(); it != node.end(); ++it) {
      ss.str("");
      ss << it->second;
      T t = ConfigCast<std::string, T>()(ss.str());
      un_map.insert(std::make_pair(it->first.Scalar(), t));
    }
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
    std::stringstream ss;
    ss << node;
    return ss.str();
  }
};

class ConfigBase
{
public:
  typedef std::shared_ptr<ConfigBase> ptr;
  ConfigBase(std::string name, std::string comment);
  const std::string &getName() const;
  const std::string &getComment() const;

  virtual std::string getTypeName() const = 0;
  virtual std::string toString() const = 0;
  virtual void fromString(const std::string &str) = 0;
  virtual void setDefault() = 0;

private:
  std::string m_name;
  std::string m_comment;
};

template<typename ConfTy, typename ToStr = ConfigCast<ConfTy, std::string>, typename FromStr = ConfigCast<std::string, ConfTy>>
class Config : public ConfigBase
{
public:
  typedef std::shared_ptr<Config<ConfTy>> ptr;
  Config(const std::string &name,
         const ConfTy &defConf,
         const std::string &comment);

  std::string getTypeName() const override;

  std::string toString() const override;

  void fromString(const std::string &str) override;

  void setValue(const ConfTy &val);
  void setValue(ConfTy &&val);
  void setDefault() override;

  const ConfTy &getValue();

private:
  std::shared_ptr<ConfTy> m_val = nullptr;
  std::shared_ptr<ConfTy> m_def = nullptr;
};

template<typename ConfTy, typename ToStr, typename FromStr>
Config<ConfTy, ToStr, FromStr>::Config(const std::string &name, const ConfTy &defConf, const std::string &comment) : ConfigBase(name, comment)
{
  m_def.reset(new ConfTy{defConf});
  m_val = m_def;
}
template<typename ConfTy, typename ToStr, typename FromStr>
std::string Config<ConfTy, ToStr, FromStr>::getTypeName() const
{
  return typeid(m_val).name();
}
template<typename ConfTy, typename ToStr, typename FromStr>
std::string Config<ConfTy, ToStr, FromStr>::toString() const
{
  try {
    return ToStr{}(*(m_val == nullptr ? m_def : m_val));
  } catch (std::exception &e) {
    std::cout << "type cast faild, "
              << "except=" << e.what() << std::endl;
  } catch (...) {
    std::cout << "type cast faild." << std::endl;
  }
  return "";
}
template<typename ConfTy, typename ToStr, typename FromStr>
void Config<ConfTy, ToStr, FromStr>::fromString(const std::string &str)
{
  try {
    *m_val = FromStr{}(str);
  } catch (std::exception &e) {
    std::cout << "type cast faild, "
              << "except=" << e.what() << std::endl;
  } catch (...) {
    std::cout << "type cast faild." << std::endl;
  }
}
template<typename ConfTy, typename ToStr, typename FromStr>
void Config<ConfTy, ToStr, FromStr>::setValue(const ConfTy &val)
{
  m_val.reset(new ConfTy{val});
}
template<typename ConfTy, typename ToStr, typename FromStr>
void Config<ConfTy, ToStr, FromStr>::setValue(ConfTy &&val)
{
  m_val.reset(new ConfTy{std::move(val)});
}
template<typename ConfTy, typename ToStr, typename FromStr>
void Config<ConfTy, ToStr, FromStr>::setDefault()
{
  m_val = m_def;
}
template<typename ConfTy, typename ToStr, typename FromStr>
const ConfTy &Config<ConfTy, ToStr, FromStr>::getValue()
{
  return *(m_val == nullptr ? m_def : m_val);
}

}// namespace myai

#endif//MY_AI_CONFIG_H
