//
// Created by HanHaocheng on 2023/11/7.
//
#ifndef MY_AI_NODE_H
#define MY_AI_NODE_H

#include <mylib/config.h>
#include <mylib/log.h>

#include <cstdint>
#include <forward_list>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace myai{

using id_t = unsigned long long;
using weight_t = float;


//=====================================================================================================================
struct Link {
  struct Hash {
    size_t operator()(const Link &link) const { return std::hash<id_t>{}(link.id); }
  };

  Link() = default;
  Link(id_t link_id, weight_t weight);
  explicit Link(const std::pair<id_t, weight_t> &pair) : id(pair.first), weight(pair.second) {}
  Link(const Link &) = default;
  Link(Link &&) = default;
  Link &operator=(const Link &) = default;
  Link &operator=(Link &&) = default;
  ~Link() = default;

  explicit operator bool() const;
  Link &operator=(const char *bytes);

  explicit operator const char *() const;
  bool operator==(const Link &rhs) const;
  bool operator!=(const Link &rhs) const;
  bool operator<(const Link &rhs) const;
  bool operator>(const Link &rhs) const;
  bool operator<=(const Link &rhs) const;
  bool operator>=(const Link &rhs) const;

  id_t id = 0;
  weight_t weight = 0;
};
//=====================================================================================================================
class LinkGroup
{
  friend class NodeDao;

public:
  using ptr = std::shared_ptr<LinkGroup>;
  template<class... Args>
  void addLink(Args &&...args);
  template<typename Iterator>
  void addLink(Iterator begin, Iterator end);

  void addLink(const Link &link);
  const std::vector<Link> &getAllLinks() const;
  void for_each(std::function<void(Link &)> func);
  void for_each(std::function<void(const Link &)> func) const;

private:
  std::vector<Link> m_datas;
};

template<class... Args>
void LinkGroup::addLink(Args &&...args)
{
  m_datas.emplace_back(std::forward<Args>(args)...);
}
template<typename Iterator>
void LinkGroup::addLink(Iterator begin, Iterator end)
{
  m_datas.insert(m_datas.end(), begin, end);
}

//=====================================================================================================================
/// 节点
/// 节点会进行输入输出
/// /
class Node
{
  friend class NodeDao;
  friend class Noder;

public:
  //可以保存激活时间戳或最后激活的次数编号，进行有规律的清除无用节点
  using ptr = std::shared_ptr<Node>;
  enum Type : int32_t
  {
    NT_UNKNOWN = -1,
    NT_MEMORY,
    NT_EMOTION
  };
  static std::string toString(Type val);
  static Type fromString(const std::string &type);
  Node(id_t id, Type type, weight_t bias);
  [[nodiscard]] id_t getId() const;
  [[nodiscard]] weight_t getBias() const;
  void setBias(weight_t bias);
  [[nodiscard]] Type getType() const;
  void setType(Type type);
  LinkGroup &linkGroup();
  [[nodiscard]] const LinkGroup &linkGroup() const;
  bool operator==(const Node &rhs) const;
  bool operator!=(const Node &rhs) const;
  bool operator<(const Node &rhs) const;
  bool operator>(const Node &rhs) const;
  bool operator<=(const Node &rhs) const;
  bool operator>=(const Node &rhs) const;

  struct Hash {
    size_t operator()(const Node &node) const { return std::hash<id_t>{}(node.m_id); }
  };

private:
  Node() = default;

private:
  id_t m_id = 0;
  weight_t m_bias = 0;
  Type m_type = Type::NT_UNKNOWN;
  bool m_isDelete = false;
  LinkGroup m_linkGroup;
};

}// namespace myai
#endif//MY_AI_NODE_H
