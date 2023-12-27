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
#include <stdexcept>
#include <string>
#include <unordered_map>
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
  bool operator==(const Link &rhs) const;
  bool operator!=(const Link &rhs) const;
  bool operator<(const Link &rhs) const;
  bool operator>(const Link &rhs) const;
  bool operator<=(const Link &rhs) const;
  bool operator>=(const Link &rhs) const;

  static uint64_t hash(const Link &link) { return std::hash<uid_t>{}(link.id); }
};
//=====================================================================================================================
class LinkGroup
{
public:
  using ptr = std::shared_ptr<LinkGroup>;
  bool addLink(const Link &link);
  template<class... Args>
  bool addLink(Args... args);
  template<typename Iterator>
  bool addLink(Iterator begin, Iterator end);
  const std::forward_list<Link> &getAllLinks() const;
  void for_each(std::function<void(Link &)> func);
  void for_each(std::function<void(const Link &)> func) const;

private:
  size_t m_linkNum = 0;
  std::forward_list<Link> m_datas;
};
//=====================================================================================================================
/// 节点
/// 节点会进行输入输出
///
/// /
class Node
{
public:
  //可以保存激活时间戳或最后激活的次数编号，进行有规律的清除无用节点
  using ptr = std::shared_ptr<Node>;
  enum Type
  {
    NT_UNKNOWN = -1,
    NT_MEMORY,
    NT_EMOTIMOAL
  };
  static std::string toString(Type val);
  static Type fromString(const std::string &type);
  Node(id_t id, Type type, weight_t bais);
  [[nodiscard]] id_t getId() const;
  [[nodiscard]] weight_t getBais() const;
  void setBais(weight_t bais);
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

  static uint64_t hash(const Node &node) { return std::hash<uid_t>{}(node.m_id); }

private:
  LinkGroup m_linkGroup;
  id_t m_id = 0;
  weight_t m_bias = 0;
  Type m_type = Type::NT_UNKNOWN;
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
  std::unordered_map<id_t, Node::ptr> m_nodeCache;
};
//=====================================================================================================================
class Activator
{
public:
  using ptr = std::shared_ptr<Activator>;
  virtual void setActivationInfo(std::unordered_map<id_t, weight_t> &link_info) = 0;
  virtual void active(id_t id, weight_t weight) = 0;
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
  explicit Noder(const NodeDao::ptr &dao, std::pair<id_t, id_t> range) {}
  ~Noder() = default;
  Node::ptr getRecverNode(weight_t bias);
  void activate();

protected:
  NodeIdAllocator::ptr m_idAlloc = nullptr;
  NodeDao::ptr m_nodeDao = nullptr;
  std::unordered_map<id_t, ActiveNode> m_cache;
  std::vector<Activator::ptr> m_activtors;
};
//=====================================================================================================================
class NodeControl
{
public:
  void init();
  void run()
  {
    while (isStop()) {
      while () {
        m_noder->activate();
      }
      learnPeriod();
    }
  }
  // 推理期
  void inferencePeriod()
  {
  }
  //学习期
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
  Noder::ptr m_noder = nullptr;
};
//=====================================================================================================================
class MemoryActivator : public Activator
{
public:
  void setActivationInfo(std::unordered_map<id_t, weight_t> &link_info) override
  {
    m_memoryLinks->for_each([&](Link &link) {
      auto &weight = link_info[link.id];
      if (weight == 0.0) {
        Node::ptr temp_node = m_nodeDao->selectById(link.id);
        if (!temp_node) {
          return;
        }
        weight += temp_node->getBais();
      }
      weight += link.weight;
    });
  }
  void active(id_t id, weight_t weight) override
  {
    const auto node = m_nodeDao->selectById(id);
    node->linkGroup().for_each([&, this](const Link &link) {
      m_memoryLinks->addLink(link.id, link.weight * weight);
    });
  }

private:
  LinkGroup::ptr m_memoryLinks;
  NodeDao::ptr m_nodeDao;
};
class EmotionalActivator : public Activator
{
public:
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
  void setActivationInfo(std::unordered_map<id_t, weight_t> &link_info) override
  {
    s_emotion.emotion_interference > 1.0
        ? link_info[m_begin + IN_POSITIVE] = (s_emotion.emotion_interference - 1.0f) * 10000.0f
        : link_info[m_begin + IN_NEGATIVE] = s_emotion.emotion_interference * 10000.0f;
    s_emotion.activates_standard > 10000.0f
        ? link_info[m_begin + IN_CONCENTRATION] = s_emotion.activates_standard
        : link_info[m_begin + IN_DISPERSION] = 10000.0f - s_emotion.activates_standard;
  }
  void active(id_t id, weight_t weight) override
  {
    auto emotionIo = (EmotionIO) (id - m_begin);
    switch (emotionIo) {
      case OUT_POSITIVE:
        s_emotion.activates_standard += weight / 10000.0f;
        break;
      case OUT_NEGATIVE:
        s_emotion.activates_standard -= weight / 10000.0f;
        break;
      case OUT_CONCENTRATION:
        s_emotion.activates_standard = std::max(s_emotion.activates_standard + weight, 20000.0f);
        break;
      case OUT_DISPERSION:
        s_emotion.activates_standard = std::max(s_emotion.activates_standard - weight, 0.0f);
        break;
      default:
        break;
    }
  }

private:
  id_t m_begin = 0;
  size_t m_size = 0;
};
}// namespace myai
#endif//MY_AI_NODE_H
