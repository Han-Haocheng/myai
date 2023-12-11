//
// Created by HanHaocheng on 2023/11/7.
//

#include "Noder.h"
#include <stdexcept>
#include <utility>

namespace myai
{

Noder::Noder(Noder::Type type, NodeDao::ptr nodeDao) : m_type(type), m_nodeDao(std::move(nodeDao)) {}
Noder::Type Noder::getType() const { return m_type; }
void Noder::setType(Noder::Type type) { m_type = type; }

MemoryNoder::MemoryNoder(NodeDao::ptr nodeDao) : Noder(Noder::Type::MEMORY, std::move(nodeDao)) {}
void MemoryNoder::input(Node::ptr link_node)
{
  m_inputNodes.clear();
  for (const auto &edge: in_infos) {
    Node::ptr tmpNode = nullptr;
    auto findRes = m_inputNodes.find(edge.first);

    if (findRes != m_inputNodes.end()) {
      tmpNode = findRes->second;
    } else {
      tmpNode = m_nodeDao->selectById(edge.first);
      m_inputNodes.emplace(edge.first, tmpNode);
    }
#ifdef DEBUG
    if (tmpNode == nullptr) {
      throw std::runtime_error{"<error MemoryNoder::input>node is not exist."};
    }
#endif
    tmpNode->setActivateValue(tmpNode->getActivateValue() + edge.second);
  }
}

void MemoryNoder::output(Node::ptr link_node)
{
  for (const auto &node: m_inputNodes) {
    uint32_t act_val = node.second->getActivateValue() + node.second->getBais();
    node.second->setActivateValue(0);
    out_infos.reserve(out_infos.size() + node.second->edges().size());
    for (const auto &edge: node.second->edges()) {
      out_infos.emplace_back(edge.first, edge.second * act_val * 0.1);
    }
  }
}
}// namespace myai