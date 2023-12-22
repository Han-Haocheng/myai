//
// Created by HanHaocheng on 2023/11/7.
//

#ifndef MY_AI_NODE_H
#define MY_AI_NODE_H

#include "../define.h"
#include <cstdint>
#include <forward_list>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace myai
{

//class NodeId
//{
//  friend class NodeIdAllocator;
//
//public:
//  NodeId(NodeId &&id) noexcept;
//  NodeId(const NodeId &) = delete;
//  NodeId &operator=(NodeId &&id) noexcept;
//  NodeId &operator=(const NodeId &) = delete;
//
//  inline explicit operator uint64_t() const;
//
//  bool operator<(const NodeId &rhs) const;
//  bool operator>(const NodeId &rhs) const;
//  bool operator<=(const NodeId &rhs) const;
//  bool operator>=(const NodeId &rhs) const;
//  bool operator==(const NodeId &rhs) const;
//  bool operator!=(const NodeId &rhs) const;
//  bool operator<(uint64_t rhs) const;
//  bool operator>(uint64_t rhs) const;
//  bool operator<=(uint64_t rhs) const;
//  bool operator>=(uint64_t rhs) const;
//  bool operator==(uint64_t rhs) const;
//  bool operator!=(uint64_t rhs) const;
//
//  NodeId() = default;
//
//private:
//  explicit NodeId(uint64_t value);
//  NodeId &operator=(uint64_t value);
//
//private:
//  uint64_t m_id = 0;
//};

using id_t = uint64_t;
using weight_t = float;

//=====================================================================================================================
class NodeIdAllocator
{
public:
  using ptr = std::shared_ptr<NodeIdAllocator>;

  explicit NodeIdAllocator(const std::string &file);
  ~NodeIdAllocator();

  id_t allocate();
  void deallocate(id_t id);

private:
  void load_from_file();
  void save_to_file();
  std::string m_file;
  id_t m_currentId = 0;
  std::forward_list<id_t> m_ids = {};
};
//=====================================================================================================================
struct Link {
  id_t id = 0;
  weight_t weight = 0;

  Link() = default;
  Link(id_t link_id, weight_t weight);

  explicit operator bool() const;
  Link &operator=(const char *bytes);
  explicit operator const char *() const;
};
//=====================================================================================================================
class LinkGroup
{
public:
  using ptr = std::shared_ptr<LinkGroup>;

  bool addLink(const Link &link);
  template<typename Iterator>
  bool addLink(Iterator begin, Iterator end);
  const std::forward_list<Link> &getAllLinks() const;
  void for_each(std::function<void(Link &)> func);

private:
  size_t m_linkNum = 0;
  std::forward_list<Link> m_datas;
};
//=====================================================================================================================
class Node
{
public:
  //可以保存激活时间戳或最后激活的次数编号，进行有规律的清除无用节点
  using ptr = std::shared_ptr<Node>;

  enum Type
  {
    NT_UNKNOWN = 0,
    NT_MEMORY = 1,
  };

  static std::string toString(Type val);
  static Type fromString(const std::string &type);

  Node(id_t id, Type type, weight_t bais);

  [[nodiscard]] id_t getId() const;
  [[nodiscard]] weight_t getBais() const;
  void setBais(weight_t bais);
  [[nodiscard]] Type getType() const;
  void setType(Type type);
  [[nodiscard]] const LinkGroup::ptr &getLinkGroup() const;
  void setLinkGroup(const LinkGroup::ptr &linkGroup);

private:
  id_t m_id = 0;
  weight_t m_bias = 0;
  Type m_type = Type::NT_UNKNOWN;
  LinkGroup::ptr m_linkGroup;
};

class ActiveNode
{
public:
  using ptr = std::shared_ptr<ActiveNode>;
  [[nodiscard]] const Node::ptr &getNode() const;
  void setNode(const Node::ptr &node);
  [[nodiscard]] size_t getNum() const;
  void setNum(size_t num);
  [[nodiscard]] weight_t getValue() const;
  void setValue(weight_t value);

private:
  Node::ptr m_node = nullptr;//节点
  size_t m_num = 0;          //激活次数
  weight_t m_value = 0;      //激活值
};
//=====================================================================================================================
class NodeDao
{
public:
  using ptr = std::shared_ptr<NodeDao>;

  int insert(const Node::ptr &node);
  int update(Node::ptr node);
  int deleteById(id_t id);
  Node::ptr selectById(id_t id);

  void clearCache();

private:
  std::unordered_map<uint64_t, Node::ptr> m_nodeCache;
};
//=====================================================================================================================
class Noder
{
public:
  using ptr = std::shared_ptr<Noder>;
  using weight_cb = std::function<double(double)>;

  explicit Noder(NodeDao::ptr dao, std::pair<id_t, id_t> range);
  ~Noder() = default;

  // for Node

  double getActivateWeight(const Node::ptr &node);

  Node::ptr getNodeById(id_t id);
  void clearCache();

  virtual std::vector<Link> &&getInput() = 0;
  virtual void setOutput(const std::vector<Link> &edges) = 0;

  void addWeightCb(const weight_cb &cb);
  void clearWeightCb();

protected:
  virtual Node::ptr getNewNode() = 0;
  void classify(const std::vector<Link> &edges, std::unordered_map<Node::Type, std::vector<Link>> &results);

private:
  std::pair<id_t, id_t> m_range;
  NodeDao::ptr m_nodeDao = nullptr;
  std::vector<weight_cb> m_weightCbs;
  std::map<id_t, Node::ptr> m_cache;
};

class MemoryNoder : public Noder
{
public:
  explicit MemoryNoder(NodeDao::ptr nodeDao);

  std::vector<Link> &&getInput() override;
  void setOutput(const std::vector<Link> &edges) override;

private:
  //std::unordered_map<uint64_t, node::ptr> m_tempNodes;
};

//=====================================================================================================================
class NodeControl
{
public:
  void init();
  void run()
  {
    while (isStop()) {
      deductionPeriod();
      learnPeriod();
    }
  }
  void deductionPeriod();
  void learnPeriod();

  bool isStop();

  void input();
  void output();

private:
  NodeIdAllocator::ptr m_idAlloc = nullptr;
  NodeDao::ptr m_nodeDao = nullptr;

  std::forward_list<Node::ptr> m_recordes = {};
  size_t m_recordeNum = 0ULL;
  size_t m_maxRecordeNum = 0ULL;

  static std::vector<Noder::ptr> s_noders;
};
}// namespace myai

#endif//MY_AI_NODE_H
