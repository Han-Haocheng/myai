//
// Created by HanHaocheng on 2024/2/22.
//
#include "NodeFactory.h"

#include <utility>

MYAI_BEGIN


NodeFactory::NodeFactory(NodeDao::ptr dao, NodeIdAllocator::ptr allocator) {
  m_node_dao = std::move(dao);
  m_allocator = std::move(allocator);
  //m_node_map.reserve(10000);
}

Node::ptr NodeFactory::createNode(Node::Type type, weight_t bias) {
  Node::ptr node{new Node{m_allocator->allocate(), type, bias}};
  m_buffer.emplace(node->id(), node);
  return node;
}

Node::ptr NodeFactory::createNode(id_t id) {
  auto fd_res = m_buffer.find(id);
  if (fd_res != m_buffer.end()) {
    return fd_res->second;
  }

  Node::ptr node = m_node_dao->selectById(id);
  if (node){
    m_buffer.emplace(node->id(), node);
  }
  return node;
}
void NodeFactory::clearBuffer() {
  for (const auto &node_pair: m_buffer) {
    m_node_dao->insert(node_pair.second, true);
  }
  m_buffer.clear();
}

MYAI_END
