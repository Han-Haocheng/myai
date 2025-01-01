#ifndef MYAI_NODE_H_
#define MYAI_NODE_H_

#include "link.h"

#include <forward_list>
#include <unordered_map>
#include <utility>

#include <mylib/config_manager.h>

MYAI_BEGIN

/// <summary>
/// node id alloc
/// </summary>
class NodeIdAlloc {
public:
  using ptr = std::shared_ptr<NodeIdAlloc>;

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  nodeid_t alloc();

  /// <summary>
  ///
  /// </summary>
  /// <param name=""></param>
  void dealloc(nodeid_t id);

private:
  nodeid_t m_current;
  std::forward_list<nodeid_t> m_free_ids;
};

/// <summary>
/// node
/// </summary>
class Node {
public:
  enum type_t {
    ND_UNDEFINED,
    ND_ORDINARY,//
    ND_EMOTION,
  };

  using ptr                                    = std::shared_ptr<Node>;

  constexpr const static weight_t DEFAULT_BIAS = 0.0;

  Node(type_t type, nodeid_t id, weight_t bias);
  inline nodeid_t id() const { return m_id; }
  inline void setId(nodeid_t id) { m_id = id; }
  inline weight_t bias() const { return m_bias; }
  inline void setBias(weight_t bias) { m_bias = bias; }
  inline type_t type() const { return m_type; }
  inline void setType(type_t type) { m_type = type; }

  inline std::vector<link_t> &links() { return m_lks; };
  inline const std::vector<link_t> &links() const { return m_lks; };
  inline weight_t &actval() { return m_activate_val; }
  inline const weight_t &actval() const { return m_activate_val; }


private:
  // 基本属性
  type_t m_type;
  nodeid_t m_id;
  weight_t m_bias;

  // 运行属性
  weight_t m_activate_val{};
  LinkStream::ptr m_links;
  std::vector<link_t> m_lks;
};

using node_t = Node::ptr;

/// <summary>
/// node dao
/// </summary>
class NodeDao {
public:
  using ptr = std::shared_ptr<NodeDao>;
  NodeDao(String url);

  node_t selectById(nodeid_t id);
  int update(node_t node);
  int insert(node_t node);
  int deleteById(nodeid_t id);

private:
  // 基本属性
  String m_url;//数据库地址
};

/// <summary>
/// node server
/// </summary>
class NodeServer {
public:
  using ptr = std::shared_ptr<NodeServer>;

  NodeServer(NodeDao::ptr dao, NodeIdAlloc::ptr alloc);
  node_t creatNode(Node::type_t type, weight_t bias);
  bool addNode(const node_t &node);
  bool setNode(const node_t &node);
  bool delNode(nodeid_t id);
  node_t getNode(nodeid_t id);

  inline void clearCache() { m_cache.clear(); }

private:
  // 基本属性
  NodeDao::ptr m_dao;
  NodeIdAlloc::ptr m_alloc;

  // 运行属性
  std::unordered_map<nodeid_t, Node::ptr> m_cache;
};

struct NodeHandlerState {
  using ptr = std::shared_ptr<NodeHandlerState>;
};

class HandlerListener {
public:
  using ptr = std::shared_ptr<HandlerListener>;
  virtual void listen(const node_t &node) = 0;

  void setRecover(node_t node) { m_recover = node; }

protected:
  node_t m_recover;
};


/// <summary>
/// node handler
/// </summary>
class NodeHandler {
public:
  using ptr = std::shared_ptr<NodeHandler>;


  NodeHandler(Node::type_t type, nodeid_t begId, size_t ndNum, NodeServer::ptr ser, NodeHandlerState::ptr state);
  virtual ~NodeHandler() = 0;

  virtual LinkStream collect() = 0;
  virtual void activate(const LinkStream::ptr &) = 0;

  void setCollectListener(HandlerListener::ptr listener) {
    m_collect_listener = listener;
  }
  void setActivateListener(HandlerListener::ptr listener) {
    m_activate_listener = listener;
  }

protected:
  // 基本属性
  Node::type_t m_type;
  nodeid_t m_begin_id;
  size_t m_node_size;
  NodeServer::ptr m_server;
  NodeHandlerState::ptr m_state;

  // 运行属性
  HandlerListener::ptr m_collect_listener;
  HandlerListener::ptr m_activate_listener;
  std::vector<Node::ptr> m_nodes;
};

class CollectNodeListener : HandlerListener {
public:
  void listen(const node_t &node) override final {
    node->links().emplace_back(m_recover->id(),0.0);//todo 数值相关
  }
};

class ActivateNodeListener : HandlerListener {
public:
  void listen(const node_t &node) override final {
    m_recover->links().emplace_back(m_recover->id(), node->actval());
  }
};

/// <summary>
/// node manager
/// </summary>
class NodeManager {
public:
  using ptr = std::shared_ptr<NodeManager>;

  explicit NodeManager(NodeServer::ptr server);
  void collectCycle();
  void activateCycle();
  void orderCycle();

  void addHandler(const NodeHandler::ptr &hd);

private:
  // 基本属性
  NodeServer::ptr m_server;
  NodeHandlerState::ptr m_state;
  HandlerListener::ptr m_collect_listener;
  HandlerListener::ptr m_activate_listener;

  std::vector<NodeHandler::ptr> m_handlers;

  // 运行属性
  node_t m_recover;
  LinkStream::ptr m_buffer;
  std::vector<Node::ptr> m_recovers;
};

/// <summary>
/// node configer 节点工厂 用于获取不同的类
/// </summary>
class NodeFactory {
public:
  using ptr = std::shared_ptr<NodeFactory>;

  NodeFactory(String path);
  NodeIdAlloc::ptr idAlloc();

  NodeServer::ptr server();
  NodeDao::ptr dao();
  std::vector<NodeHandler::ptr> &handlers();
  NodeHandlerState::ptr handlerStare();

  NodeManager::ptr manager();

private:
  Configurator::ptr m_config;
  String m_path;
};

MYAI_END

#endif// !MYAI_NODE_H_