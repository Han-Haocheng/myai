//
// Created by HanHaocheng on 2023/11/30.
//

#include "ThinkControl.h"

namespace myai
{

void ThinkControl::init()
{
  m_nodeDao = std::make_shared<NodeDao>();
  m_idAlloc = std::make_shared<NodeIdAllocator>();
  m_nodeFactory = std::make_shared<NodeFactory>(m_idAlloc);
  s_noderMap.emplace(Noder::Type::MEMORY, std::make_shared<MemoryNoder>(m_nodeDao));
}

void ThinkControl::run()
{
  while (!stop()) {
    while (!timeout()) {
      std::unordered_map<Noder::Type, std::vector<Node::Edge>> out_infos;
      std::unordered_map<Noder::Type, std::vector<Node::Edge>> in_infos;
      auto recorde = m_nodeFactory->getInstance(256, Node::Type::HIDDEN_NODE);
      m_nodeDao->insert(recorde);
      m_recordes.emplace_front(recorde);
      ++m_recordeNums;

      for (const auto &noder: s_noderMap) {
        noder.second->output(out_infos[noder.first]);
      }
      for (const auto &item: out_infos) {
        for (const auto &edge: item.second) {
          in_infos[item.first].emplace_back(edge);
        }
      }
      for (const auto &noder: s_noderMap) {
        noder.second->input(in_infos[noder.first]);
      }
      recorde->edges();
    }
    for (const auto &node: m_recordes) {

      for (const auto &edge: node->edges()) {
        auto tmpNode = m_nodeDao->selectById(edge.first);
        if (!tmpNode) {
          continue;
        }
        tmpNode->edges();
      }
    }
    m_recordes.clear();
  }
}
}// namespace myai