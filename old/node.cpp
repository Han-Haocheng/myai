#include "node.h"
#include <mylib/exception.h>
#include <utility>

MYAI_BEGIN

nodeid_t myai::NodeIdAlloc::alloc() {
  nodeid_t res;
  if (m_free_ids.empty()) {
    res = m_free_ids.front();
    m_free_ids.pop_front();
  } else {
    res = m_current;
    ++m_current;
  }
  return res;
}

void NodeIdAlloc::dealloc(nodeid_t id) {
  m_free_ids.emplace_front(id);
}


Node::Node(Node::type_t type, nodeid_t id, weight_t bias)
    : m_type(type), m_id(id), m_bias(bias) {
  if (id == UNDEFINED_NODE_ID) {
    MYLIB_THROW("id undef");
  }
  if (type == Node::ND_UNDEFINED) {
    MYLIB_THROW("type undef");
  }
}

NodeServer::NodeServer(NodeDao::ptr dao, NodeIdAlloc::ptr alloc)
    : m_dao(std::move(dao)), m_alloc(std::move(alloc)) {
  if (!m_dao || !m_alloc) {
    MYLIB_THROW("m_dao or m_alloc is null");
  }
}

node_t NodeServer::creatNode(Node::type_t type, weight_t bias) {
  nodeid_t id = m_alloc->alloc();
  node_t node = std::make_shared<Node>(type, id, bias);
  return node;
}

bool NodeServer::addNode(const node_t &node) {
  // 检查存在
  if (!node) {
    MYLIB_THROW("node is null");
  }
  return m_dao->insert(node) > 0;
}

bool NodeServer::setNode(const node_t &node) {
  if (!node) {
    MYLIB_THROW("node is null");
  }
  return m_dao->update(node) > 0;
}
bool NodeServer::delNode(nodeid_t id) {
  if (id == UNDEFINED_NODE_ID) {
    MYLIB_THROW("id undef");
  }
  auto rt = m_dao->deleteById(id);
  if (rt) {
    m_alloc->dealloc(id);
  }
  return rt;
}
node_t NodeServer::getNode(nodeid_t id) {
  if (id == UNDEFINED_NODE_ID) {
    MYLIB_THROW("id undef");
  }
  node_t node = nullptr;
  auto fd_rt  = m_cache.find(id);
  if (fd_rt != m_cache.end()) {
    node = fd_rt->second;
  } else {
    node = m_dao->selectById(id);
    if (node) {
      m_cache.emplace(node->id(), node);
    }
  }
  return node;
}

NodeHandler::NodeHandler(Node::type_t type,
                         myai::nodeid_t begId,
                         size_t ndNum,
                         NodeServer::ptr ser,
                         NodeHandlerState::ptr state)
    : m_type(type), m_begin_id(begId), m_node_size(ndNum), m_server(std::move(ser)), m_state(std::move(state)) {
  if (m_type == Node::ND_UNDEFINED) {
    MYLIB_THROW("type undef");
  }
  if (!m_server) {
    MYLIB_THROW("server is null");
  }
  if (!m_state) {
    MYLIB_THROW("state is null");
  }
}

inline void CollectNodeListener::listen(const node_t &node) {
  node->links().emplace_back(m_recover->id(), 0.0);//todo 数值相关
}

inline void ActivateNodeListener::listen(const node_t &node) {
  m_recover->links().emplace_back(m_recover->id(), node->actval());
}

NodeManager::NodeManager(NodeServer::ptr server)
    : m_state(std::make_shared<NodeHandlerState>()),
      m_server(std::move(server)) {
}

void NodeManager::collectCycle() {
  for (const auto &handler: m_handlers) {
    *m_buffer << handler->collect();
  }
}

void NodeManager::activateCycle() {
  m_recover = m_server->creatNode(Node::ND_UNDEFINED, 0.0);//todo 数值相关
  m_collect_listener->setRecover(m_recover);
  m_activate_listener->setRecover(m_recover);

  for (const auto &handler: m_handlers) {
    handler->activate(m_buffer);
  }
}

void NodeManager::orderCycle() {
  //todo
}

void NodeManager::addHandler(const NodeHandler::ptr &hd) {
  if (!hd) {
    MYLIB_THROW("hd is null");
  }
  m_handlers.emplace_back(hd);
}

MYAI_END

