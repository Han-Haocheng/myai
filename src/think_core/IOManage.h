//
// Created by HanHaocheng on 2023/05/16.
//

#ifndef THINKSYSTEM_IOMANAGE_H
#define THINKSYSTEM_IOMANAGE_H
#include "inout/IOAction.h"
#include "inout/IOAudio.h"
#include "inout/IOInterface.h"
#include "inout/IOMemory.h"
#include "inout/IOVideo.h"

namespace think
{

/// \b 多输入输出 感知-单节点-多节点-行为 双层节点架构
class IOManage
{
private:
  NodeIdAllocator m_idAlloca;
  std::unordered_map<node_id, ActFunc> m_mapAction_;
  std::vector<std::shared_ptr<io::IOInterface>> m_lstIOS_;

  std::unordered_map<node_id, std::shared_ptr<NodeInfo>> m_nodeBuffer_;//节点缓存


  std::unique_ptr<NodeInfo>m_actedLinks_;

  /*
  LinkList m_actedDynamic_;
  LinkList m_actedStatic_;
  LinkList m_actedConst_;*/

  constexpr static const link_val NUM_DEF_DESIRE = 0x800;
  constexpr static const link_val NUM_DEF_MEMORY = 0x800;

public:
  active_t m_focusStand_ = 0x800;

public:
  size_t getNodeBufferSize() { return m_nodeBuffer_.size(); }
  IOManage() : m_idAlloca(NUM_NODE_ID_MAX) { initConstType(); }

  void initConstType();
  void loadLink(std::unordered_map<node_id, std::shared_ptr<NodeInfo>> &actBuffer, const link_t &lk);
  size_t runLoop();
  void stableThink();

private:
};//! IOManage

inline void IOManage::initConstType()
{
  m_lstIOS_.emplace_back(std::make_unique<io::IOMemory>());
  m_lstIOS_.emplace_back(std::make_unique<io::IOVideo>());
  m_lstIOS_.emplace_back(std::make_unique<io::IOAudio>());
  m_lstIOS_.emplace_back(std::make_unique<io::IOAction>());
  for (const auto &item: m_lstIOS_)
  {
    item->initConstId(m_idAlloca, m_nodeBuffer_);
    item->loadActFunc(m_mapAction_);
  }
}
inline void IOManage::loadLink(std::unordered_map<node_id, std::shared_ptr<NodeInfo>> &actBuffer, const link_t &lk)
{
  auto findRes = actBuffer.find(lk.id);
  if (findRes != actBuffer.end()) { findRes->second->actVal += lk.linkVal; }
  else
  {
    auto findRes2 = m_nodeBuffer_.find(lk.id);
    if (findRes2 != m_nodeBuffer_.end())
    {
      findRes2->second->actVal = lk.linkVal;
      actBuffer.emplace(lk.id, findRes2->second);
    }
    else
    {
      node_type &empRes     = *m_nodeBuffer_.emplace(lk.id, std::make_unique<NodeInfo>(lk.id)).first;
      empRes.second->actVal = lk.linkVal;
      actBuffer.emplace(lk.id, empRes.second);
    }
  }
}
inline size_t IOManage::runLoop()
{
  std::unordered_map<node_id, std::shared_ptr<NodeInfo>> m_mapActiveNode;//激活节点缓存

  node_type &firstNode = *m_nodeBuffer_.emplace(m_idAlloca.allocate(), std::make_unique<NodeInfo>()).first;
  for (const auto &item: m_lstIOS_)
  {
    for (const link_t &lk: (*item->inputActInfo())) { loadLink(m_mapActiveNode, lk); }
  }

  for (const link_t &lk: m_actedLinks_->m_lsStaticLink) loadLink(m_mapActiveNode, lk);

  for (const link_t &lk: m_actedLinks_->m_lsDynamicLink) loadLink(m_mapActiveNode, lk);

  firstNode.second = std::move(m_actedLinks_);

  m_actedLinks_ = std::make_unique<NodeInfo>(ENodeType::DYNAMIC_TYPE);
  for (const auto &[id, info]: m_mapActiveNode)
  {
    info->m_lsDynamicLink.emplace(firstNode.first, NUM_DEF_MEMORY);
    info->act(io::IOInterface::interference.emotionActive,
              io::IOInterface::interference.mentalConcentration * m_focusStand_,
              *m_actedLinks_);
  }
  return m_mapActiveNode.size();
}
inline void IOManage::stableThink()
{
  for (const auto &[id, info]: m_nodeBuffer_)
  {
    info->stableLink(0.5);
    info->writeNodeInfo(id);
  }
  m_nodeBuffer_.clear();
  for (const auto &item: m_lstIOS_) item->initConstId(m_idAlloca, m_nodeBuffer_);
}

}// namespace think

#endif//THINKSYSTEM_IOMANAGE_H
