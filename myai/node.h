//
// Created by HanHaocheng on 2023/11/7.
//
#ifndef MY_AI_NODE_H
#define MY_AI_NODE_H
#include "../mylib/config.h"
#include "../mylib/log.h"
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
namespace myai
{
using id_t = uint64_t;
using weight_t = float;
//=====================================================================================================================
class NodeIdAllocator
{
public:
  using ptr = std::shared_ptr<NodeIdAllocator>;
  explicit NodeIdAllocator(std::string file);
  explicit NodeIdAllocator(id_t begin, const std::forward_list<id_t> &ids);
  ~NodeIdAllocator();
  id_t allocate();
  void deallocate(id_t id);

  id_t allocate(size_t i);

private:
  void load_from_file();
  void save_to_file();
  std::string m_file;
  id_t m_currentId = 0;
  std::forward_list<id_t> m_ids = {};
};
//=====================================================================================================================
struct Link {
  struct Hash {
    size_t operator()(const Link &link) const { return std::hash<uid_t>{}(link.id); }
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
  const std::forward_list<Link> &getAllLinks() const;
  void for_each(std::function<void(Link &)> func);
  void for_each(std::function<void(const Link &)> func) const;

private:
  size_t m_linkNum = 0;
  std::forward_list<Link> m_datas;
};
template<class... Args>
void LinkGroup::addLink(Args &&...args)
{
  m_linkNum++;
  m_datas.emplace_front(std::forward<Args>(args)...);
}
template<typename Iterator>
void LinkGroup::addLink(Iterator begin, Iterator end)
{
  m_linkNum += std::distance(begin, end);
  m_datas.insert_after(m_datas.before_begin(), begin, end);
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
//=====================================================================================================================
class NodeDao
{
public:
  using ptr = std::shared_ptr<NodeDao>;
  explicit NodeDao(std::string dataRootPath = "./data");
  int insert(const Node::ptr &node, bool isForced = false);
  int update(const Node::ptr &node);

  int deleteById(id_t id);
  Node::ptr selectById(id_t id);

private:
  std::string path_parse(id_t id);

private:
  std::string m_rootPath = ".";
};
//=====================================================================================================================
class Activator
{
public:
  using ptr = std::shared_ptr<Activator>;
  explicit Activator(Node::Type type);
  virtual ~Activator() = default;
  virtual void setActivationInfo(std::unordered_map<id_t, weight_t> &link_info) = 0;
  virtual void active(const Link &link) = 0;

private:
  Node::Type m_type = Node::NT_UNKNOWN;
};

//=====================================================================================================================
class Noder
{
public:
  using ptr = std::shared_ptr<Noder>;
  struct ActiveNode {
    Node::ptr node = nullptr;
    Activator::ptr activator;
    weight_t active_val = 0;
  };
  explicit Noder(NodeDao::ptr dao, NodeIdAllocator::ptr idAlloc);
  ~Noder() = default;
  Node::ptr addRecordNode(weight_t bias);
  id_t addNodes(Node::Type type, size_t size, std::vector<Node::ptr> &out_nodes);

  Node::ptr getNode(id_t id);
  bool delNode(id_t id);

  void activate(const Node::ptr &record_node, const Node::ptr &next_node);

  void addActivator(const Activator::ptr &activator);
  void clearActivator();

private:
  void clear_cache();

protected:
  NodeIdAllocator::ptr m_idAlloc = nullptr;
  NodeDao::ptr m_nodeDao = nullptr;
  std::unordered_map<id_t, ActiveNode> m_activeCache;
  std::unordered_map<id_t, Node::ptr> m_nodeCache;
  std::vector<Activator::ptr> m_activtors;
};

//=====================================================================================================================

struct ThinkConfig;
class NodeControl
{
public:
  using ptr = std::shared_ptr<NodeControl>;

  void init();

  void run();
  // 推理期
  void inferencePeriod();
  //学习期
  void learnPeriod();
  static bool isStop();

  static NodeControl::ptr getInstance();

private:
  NodeControl();

private:
  NodeIdAllocator::ptr m_idAlloc = nullptr;
  NodeDao::ptr m_nodeDao = nullptr;
  Noder::ptr m_noder = nullptr;

  std::forward_list<Node::ptr> m_records = {};
  size_t m_recordeNum = 0ULL;
  size_t m_maxRecordeNum = 0ULL;

  static ::mylib::Config<ThinkConfig>::ptr s_think_conf;
};
//=====================================================================================================================
class MemoryActivator : public Activator
{
public:
  explicit MemoryActivator(NodeDao::ptr nodeDao);
  ~MemoryActivator() override = default;
  void setActivationInfo(std::unordered_map<id_t, weight_t> &link_info) override;

  void active(const Link &link) override;

private:
  LinkGroup::ptr m_memoryLinks;
  NodeDao::ptr m_nodeDao;
};

class EmotionalActivator : public Activator
{
public:
  const static size_t NODE_NUMS = 8ULL;

  enum EmotionIO
  {
    IN_POSITIVE,
    IN_NEGATIVE,
    IN_CONCENTRATION,
    IN_DISPERSION,
    OUT_POSITIVE,
    OUT_NEGATIVE,
    OUT_CONCENTRATION,
    OUT_DISPERSION,
  };

  static struct emotion {
    weight_t emotion_interference;
    weight_t activates_standard;
  } s_emotion;

  explicit EmotionalActivator(id_t id_begin);
  ~EmotionalActivator() override = default;

  void setActivationInfo(std::unordered_map<id_t, weight_t> &link_info) override;

  void active(const Link &link) override;

private:
  id_t m_begin = 0;
};
}// namespace myai
#endif//MY_AI_NODE_H
