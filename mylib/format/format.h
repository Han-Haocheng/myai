#ifndef MY_LIB_FORMAT_H_
#define MY_LIB_FORMAT_H_

#include "../define/type.h"
#include <boost/lexical_cast.hpp>
#include <forward_list>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <yaml-cpp/yaml.h>

#define MYLIB_FORMAT_BEGIN MYLIB_BEGIN namespace format {
#define MYLIB_FORMAT_END \
  MYLIB_END              \
  }

MYLIB_FORMAT_BEGIN

template<class T>
class Formatter {
public:
  using FormatType = T;
  std::string toString(const FormatType &val) { return boost::lexical_cast<std::string>(val); }
  FormatType fromString(const std::string &str) { return boost::lexical_cast<FormatType>(str); }
};

template<class T>
class Formatter<std::vector<T>> {
public:
  std::string toString(const std::vector<T> &vec) {
    YAML::Node node;
    for (const auto &item: vec)
      node.push_back(YAML::Load(Formatter<T>().toString(item)));
    return YAML::Dump(node);
  }
  std::vector<T> fromString(const std::string &str) {
    YAML::Node node = YAML::Load(str);
    typename std::vector<T> vec;
    for (auto &&item_str: node)
      vec.emplace_back(Formatter<T>().fromString(YAML::Dump(item_str)));
    return vec;
  }
};
template<class T>
class Formatter<std::list<T>> {
public:
  std::string toString(const std::list<T> &list) {
    YAML::Node node;
    for (const auto &i: list)
      node.push_back(YAML::Load(Formatter<T>().toString(i)));
    return YAML::Dump(node);
  }
  std::list<T> fromString(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::list<T> list;
    for (auto &&i: node)
      list.emplace_back(Formatter<T>{}.fromString(YAML::Dump(i)));
    return list;
  }
};

template<class T>
class Formatter<std::forward_list<T>> {
public:
  std::string toString(const std::forward_list<T> &fwd_list) {
    YAML::Node node;
    for (const auto &i: fwd_list)
      node.push_back(YAML::Load(Formatter<T>().toString(i)));
    return YAML::Dump(node);
  }

  std::forward_list<T> fromString(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::forward_list<T> list;
    for (auto &&i: node)
      list.emplace_front(Formatter<T>().fromString(Dump(i)));
    return list;
  }
};

template<class T>
class Formatter<std::set<T>> {
public:
  std::string toString(const std::set<T> &set) {
    YAML::Node node;
    for (const auto &i: set)
      node.push_back(YAML::Load(Formatter<T>().toString(i)));
    return YAML::Dump(node);
  }
  std::set<T> fromString(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::set<T> set;
    for (auto &&i: node)
      set.insert(Formatter<T>().fromString(YAML::Dump(i)));
    return set;
  }
};

template<class T>
class Formatter<std::unordered_set<T>> {
public:
  std::string toString(const std::unordered_set<T> &un_set) {
    YAML::Node node;
    for (const auto &i: un_set)
      node.push_back(YAML::Load(Formatter<T>().toString(i)));
    return YAML::Dump(node);
  }
  std::unordered_set<T> fromString(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::unordered_set<T> un_set;
    for (auto &&i: node)
      un_set.insert(Formatter<T>().fromString(YAML::Dump(i)));
    return un_set;
  }
};

template<class T>
class Formatter<std::map<std::string, T>> {
public:
  std::string toString(const std::map<std::string, T> &map) {
    YAML::Node node;
    for (auto &val: map)
      node[val.first] = YAML::Load(Formatter<T>().toString(val.second));
    return YAML::Dump(node);
  }
  std::map<std::string, T> fromString(const std::string &str) {
    YAML::Node nodes = YAML::Load(str);
    typename std::map<std::string, T> map;
    for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it)
      map.insert(std::make_pair(node_it->first.Scalar(),
                                Formatter<T>().fromString(YAML::Dump(node_it->second))));
    return map;
  }
};

template<class T>
class Formatter<std::unordered_map<std::string, T>> {
public:
  std::string toString(const std::unordered_map<std::string, T> &un_map) {
    YAML::Node node;
    for (auto &i: un_map) {
      node[i.first] = YAML::Load(Formatter<T>().toString(i.second));
    }
    return YAML::Dump(node);
  }

  std::unordered_map<std::string, T> fromString(const std::string &v) {
    YAML::Node node = YAML::Load(v);
    typename std::unordered_map<std::string, T> un_map;
    for (auto it = node.begin(); it != node.end(); ++it)
      un_map.insert(std::make_pair(it->first.Scalar(),
                                   Formatter<T>().fromString(YAML::Dump(it->second))));
    return un_map;
  }
};

MYLIB_FORMAT_END
#endif
