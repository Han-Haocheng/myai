////
//// Created by HanHaocheng on 2023/9/27.
////
//
//#ifndef MY_AI_NODE_H
//#define MY_AI_NODE_H
//
<<<<<<< HEAD

#ifndef MY_AI_NODE_H
#define MY_AI_NODE_H

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "singleton.h"

namespace myai
{

typedef size_t id_t;

class NodeType
{
public:
  enum Type : uint32_t
  {
    UNKNOWN,
    BASIC_NODE, //基础节点
    INPUT_NODE, //输入节点
    OUTPUT_NODE,//输出节点
  };
};
//=====================================================================================================================

class Edge
{
public:
  typedef id_t IdType;

  Edge(IdType id, double weight);
  std::pair<IdType, double> toPair();
  void fromPair(const std::pair<IdType, double> &v);

  IdType getId() const { return m_id; }
  double getWeight() const { return m_weight; }
  double &getWeight() { return m_weight; }

private:
  IdType m_id;
  double m_weight;
};

typedef std::vector<Edge> EdgeBuffer;
class NodeActivater;

class Node
{
public:
  typedef id_t IdType;

  typedef std::shared_ptr<Node> ptr;
  Node(Node::IdType id, NodeType::Type type, double bias);

  bool operator==(const Node &rhs) const;
  bool operator!=(const Node &rhs) const;

  IdType getId() const { return m_id; }
  double getBias() const { return m_bias; }
  double setBias(double bias) { m_bias = bias; }

  EdgeBuffer &nextEdges() { return m_nextEdges; }
  EdgeBuffer &preEdges() { return m_preEdges; }
  double &actWeight() { return m_actWeight; }

  NodeType::Type getType() const { return m_type; }
  void setActivater(NodeType::Type activater) { m_type = activater; }

private:
  const IdType m_id = 0;
  NodeType::Type m_type = NodeType::UNKNOWN;
  double m_bias = 0;
  double m_actWeight = 0;
  EdgeBuffer m_nextEdges;
  EdgeBuffer m_preEdges;
  //std::shared_ptr<NodeActivater> m_activater = nullptr;
};
//=====================================================================================================================

class NodeBuffer
{
public:
  typedef id_t IdType;

  typedef std::shared_ptr<NodeBuffer> ptr;
  typedef std::map<IdType, Node::ptr> NodeMap;

  void addNode(const Node::ptr &node);
  void delNode(IdType id);
  void clear();

  Node::ptr getNode(IdType id);
  NodeMap &getAllNodes() { return m_nodes; }

private:
  NodeMap m_nodes;
};

//=====================================================================================================================
// 节点激活器，用于提供节点功能
class NodeActivater
{
public:
  typedef std::shared_ptr<NodeActivater> ptr;

  virtual ~NodeActivater() = default;
  virtual void init() = 0;
  virtual void activate(Node::ptr acted_node, NodeBuffer::ptr act_res) = 0;
};

class BaseActivater : public NodeActivater
{
public:
  ~BaseActivater() override = default;

  void init() override;
  void activate(Node::ptr acted_node, NodeBuffer::ptr act_res) override;

private:
};

class SentimentNodeActivater : public NodeActivater
{
public:
  void init() override;
  void activate(Node::ptr acted_node, NodeBuffer::ptr act_res) override;

private:
  std::map<uint64_t, std::function<void(double)>> m_outputMap;
};

//=====================================================================================================================

class Recorder
{
public:
  typedef std::shared_ptr<Recorder> ptr;

  explicit Recorder(NodeBuffer::ptr record_nodes);
  Recorder() = default;
  virtual ~Recorder() = default;
  virtual void record(NodeBuffer::ptr) = 0;

  NodeBuffer::ptr getRecords() const { return m_recordeds; }
  void setRecords(const NodeBuffer::ptr &nodes) { m_recordeds = nodes; }
  static Node::ptr UpdateNode(NodeType::Type type, double bais);

protected:
  static Node::ptr s_shareNode;
  NodeBuffer::ptr m_recordeds;//记录节点群
};

class PreRecorder : public Recorder
{
public:
  PreRecorder() = default;
  using Recorder::Recorder;
  void record(NodeBuffer::ptr ptr) override;
};

class NextRecorder : public Recorder
{
public:
  NextRecorder() = default;
  void record(NodeBuffer::ptr ptr) override;
};

//节点器管理，用于管理节点器
class NoderManager
{
public:
  //  class Recorded
  //  {
  //  public:
  //    virtual void recorded(NodeBuffer::ptr, Node::ptr) = 0;
  //  };

  typedef std::shared_ptr<NoderManager> ptr;
  typedef std::function<void(NodeBuffer::ptr, Node::ptr)> active_node_cb;

  NodeBuffer::ptr getRootNodes() const { return m_rootNodes; }

  Node::ptr lookup(uint64_t id) { return m_rootNodes->getNode(id); }
  Node::ptr lookup(double bias, NodeType::Type type);

  void activate();
  void study();

  static ptr GetInstance();

private:
  void init();
  NoderManager() { init(); }

private:
  NodeBuffer::ptr m_actNodes; //活跃节点
  NodeBuffer::ptr m_rootNodes;//根节点

  Recorder::ptr m_preRecorder;
  Recorder::ptr m_nextRecorder;
  std::map<NodeType::Type, NodeActivater::ptr> m_activaters;
};

//typedef Singleton<NoderManager> NodeMgr;

//=====================================================================================================================

}// namespace myai
#endif//MY_AI_NODE_H
=======
//#include <functional>
//#include <map>
//#include <memory>
//#include <string>
//#include <unordered_map>
//#include <utility>
//#include <vector>
//
//
//
//namespace myai
//{
//
//typedef size_t id_t;
//
//class Type
//{
//public:
//  enum Type : uint32_t
//  {
//    NT_UNKNOWN,
//    BASIC_NODE, //基础节点
//    INPUT_NODE, //输入节点
//    OUTPUT_NODE,//输出节点
//  };
//};
////=====================================================================================================================
//
//class Link
//{
//public:
//  typedef id_t IdType;
//
//  Link(IdType id, double m_weight);
//  std::pair<IdType, double> toPair();
//  void fromPair(const std::pair<IdType, double> &v);
//
//  IdType getLinkId() const { return id; }
//  double getWeight() const { return m_weight; }
//  double &getWeight() { return m_weight; }
//
//private:
//  IdType id;
//  double m_weight;
//};
//
//typedef std::vector<Link> EdgeBuffer;
//class NodeActivater;
//
//class Node
//{
//public:
//  typedef id_t IdType;
//
//  typedef std::shared_ptr<Node> ptr;
//  Node(Node::IdType id, Type::Type type, double bias);
//
//  bool operator==(const Node &rhs) const;
//  bool operator!=(const Node &rhs) const;
//
//  IdType getId() const { return id; }
//  double getBias() const { return m_bias; }
//  double setBias(double bias) { m_bias = bias; }
//
//  EdgeBuffer &nextEdges() { return m_nextEdges; }
//  EdgeBuffer &preEdges() { return m_preEdges; }
//  double &actWeight() { return m_actWeight; }
//
//  Type::Type getType() const { return m_type; }
//  void setActivater(Type::Type activater) { m_type = activater; }
//
//private:
//  const IdType id = 0;
//  Type::Type m_type = Type::NT_UNKNOWN;
//  double m_bias = 0;
//  double m_actWeight = 0;
//  EdgeBuffer m_nextEdges;
//  EdgeBuffer m_preEdges;
//  //std::shared_ptr<NodeActivater> m_activater = nullptr;
//};
////=====================================================================================================================
//
//class NodeBuffer
//{
//public:
//  typedef id_t IdType;
//
//  typedef std::shared_ptr<NodeBuffer> ptr;
//  typedef std::map<IdType, Node::ptr> NodeMap;
//
//  void addNode(const Node::ptr &node);
//  void delNode(IdType id);
//  void clear();
//
//  Node::ptr getNode(IdType id);
//  NodeMap &getAllNodes() { return m_nodes; }
//
//private:
//  NodeMap m_nodes;
//};
//
////=====================================================================================================================
//// 节点激活器，用于提供节点功能
//class NodeActivater
//{
//public:
//  typedef std::shared_ptr<NodeActivater> ptr;
//
//  virtual ~NodeActivater() = default;
//  virtual void init() = 0;
//  virtual void activate(Node::ptr acted_node, NodeBuffer::ptr act_res) = 0;
//};
//
//class BaseActivater : public NodeActivater
//{
//public:
//  ~BaseActivater() override = default;
//
//  void init() override;
//  void activate(Node::ptr acted_node, NodeBuffer::ptr act_res) override;
//
//private:
//};
//
//class SentimentNodeActivater : public NodeActivater
//{
//public:
//  void init() override;
//  void activate(Node::ptr acted_node, NodeBuffer::ptr act_res) override;
//
//private:
//  std::map<uint64_t, std::function<void(double)>> m_outputMap;
//};
//
////=====================================================================================================================
//
//class Recorder
//{
//public:
//  typedef std::shared_ptr<Recorder> ptr;
//
//  explicit Recorder(NodeBuffer::ptr record_nodes);
//  Recorder() = default;
//  virtual ~Recorder() = default;
//  virtual void record(NodeBuffer::ptr) = 0;
//
//  NodeBuffer::ptr getRecords() const { return m_recordeds; }
//  void setRecords(const NodeBuffer::ptr &nodes) { m_recordeds = nodes; }
//  static Node::ptr UpdateNode(Type::Type type, double getBias);
//
//protected:
//  static Node::ptr s_shareNode;
//  NodeBuffer::ptr m_recordeds;//记录节点群
//};
//
//class PreRecorder : public Recorder
//{
//public:
//  PreRecorder() = default;
//  using Recorder::Recorder;
//  void record(NodeBuffer::ptr ptr) override;
//};
//
//class NextRecorder : public Recorder
//{
//public:
//  NextRecorder() = default;
//  void record(NodeBuffer::ptr ptr) override;
//};
//
////节点器管理，用于管理节点器
//class NoderManager
//{
//public:
//  //  class Recorded
//  //  {
//  //  public:
//  //    virtual void recorded(NodeBuffer::ptr, node::ptr) = 0;
//  //  };
//
//  typedef std::shared_ptr<NoderManager> ptr;
//  typedef std::function<void(NodeBuffer::ptr, Node::ptr)> active_node_cb;
//
//  NodeBuffer::ptr getRootNodes() const { return m_rootNodes; }
//
//  Node::ptr lookup(uint64_t id) { return m_rootNodes->getNode(id); }
//  Node::ptr lookup(double bias, Type::Type type);
//
//  void activate();
//  void study();
//
//  static ptr GetInstance();
//
//private:
//  void init();
//  NoderManager() { init(); }
//
//private:
//  NodeBuffer::ptr m_actNodes; //活跃节点
//  NodeBuffer::ptr m_rootNodes;//根节点
//
//  Recorder::ptr m_preRecorder;
//  Recorder::ptr m_nextRecorder;
//  std::map<Type::Type, NodeActivater::ptr> m_activaters;
//};
//
////typedef Singleton<NoderManager> NodeMgr;
//
////=====================================================================================================================
//
//}// namespace myai
//#endif//MY_AI_NODE_H
>>>>>>> da2123e (v0.5.1)
