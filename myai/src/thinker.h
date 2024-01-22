#ifndef MY_AI_THINKER_H
#define MY_AI_THINKER_H
#include <string>
#include <mylib/config.h>
#include "node.h"

namespace myai
{
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

struct ThinkConfig {
  struct NodeIdAllocatorConfig {
    myai::id_t currentId;
    std::forward_list<myai::id_t> ids;
  };
  struct NoderConfig {
    Node::Type type;
    id_t idStart;
    size_t idNum;
  };
  NodeIdAllocatorConfig idAllocator;
  std::vector<NoderConfig> noders;
  size_t maxRecordeNum;
};

}// namespace myai

namespace mylib
{
template<>
struct Formatter<myai::ThinkConfig> {
  myai::ThinkConfig fromString(const std::string &val)
  {
    YAML::Node node = YAML::Load(val);
    myai::ThinkConfig ret{};
    if (node["idAllocator"].IsDefined()) {
      if (node["idAllocator"]["currentId"].IsDefined()) {
        ret.idAllocator.currentId = node["idAllocator"]["currentId"].as<myai::id_t>();
      }
      if (node["idAllocator"]["ids"].IsDefined()) {
        ret.idAllocator.ids = id_lst.fromString(Dump(node["idAllocator"]["ids"]));
      }
    }
    if (node["noders"].IsDefined()) {
      for (const auto &processor_node: node["noders"]) {
        ret.noders.emplace_back(myai::ThinkConfig::NoderConfig{
            myai::Node::fromString(processor_node["type"].as<std::string>()),
            processor_node["idStart"].as<myai::id_t>(),
            processor_node["idNum"].as<size_t>(),
        });
      }
    }
    return ret;
  }

  std::string toString(const myai::ThinkConfig &val)
  {
    YAML::Node node;
    if (val.idAllocator.currentId != 0) {
      node["idAllocator"]["currentId"] = val.idAllocator.currentId;
    }
    node["idAllocator"]["ids"] = id_lst.toString(val.idAllocator.ids);
    for (const auto &item: val.noders) {
      YAML::Node processor_node;
      processor_node["type"] = myai::Node::toString(item.type);
      processor_node["idStart"] = item.idStart;
      processor_node["idNum"] = item.idNum;
      node["noders"].push_back(processor_node);
    }
    return YAML::Dump(node);
  }
  Formatter<std::forward_list<myai::id_t>> id_lst{};
};

}// namespace mylib

#endif// MY_AI_THINKER_H