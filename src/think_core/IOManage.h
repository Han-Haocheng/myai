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
  struct ActNodeInfo
  {
    std::shared_ptr<NodeInfo> info;
    active_t actVal = 0;
    explicit ActNodeInfo(const std::shared_ptr<NodeInfo> &info, const link_val actVal = 0) : info(info), actVal(actVal)
    {
    }

    void act(LinkList &lsDynamic, LinkList &lsStatic, LinkList &lsConst, active_t stand,
             const WEIGHT<link_val> &emotionActive) const
    {
      if (stand > actVal) { return; }
      double saveVal = static_cast<double>(actVal) * emotionActive.val() / NUM_LINK_VAL_MAX;
      lsDynamic.weight(info->m_lsDynamicLink, saveVal);
      lsStatic.weight(info->m_lsStaticLink, saveVal);
      lsConst.weight(info->m_lsConstLink, saveVal);
    }
  };

private:
  NodeIdAllocator m_idAlloca;
  std::unordered_map<node_id, ActFunc> m_mapAction_;
  std::vector<std::shared_ptr<io::IOInterface>> m_lstIOS_;

  std::unordered_map<node_id, std::shared_ptr<NodeInfo>> m_nodeBuffer_;//节点缓存

  LinkList m_actedDynamic_;
  LinkList m_actedStatic_;
  LinkList m_actedConst_;

  constexpr static const link_val NUM_DEF_DESIRE = 0x800;
  constexpr static const link_val NUM_DEF_MEMORY = 0x800;

public:
  active_t m_focusStand_ = 0x800;

public:
  size_t getNodeBufferSize() { return m_nodeBuffer_.size(); }
  IOManage() : m_idAlloca(NUM_NODE_ID_MAX) { initConstType(); }

  void initConstType()
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

  size_t runLoop()
  {
    std::unordered_map<node_id, ActNodeInfo> m_mapActiveNode;//激活节点缓存
    auto loadLink = [&](const link_t &lk) -> void
    {
      auto findRes = m_mapActiveNode.find(lk.id);
      if (findRes != m_mapActiveNode.end()) { findRes->second.actVal += lk.linkVal; }
      else
      {
        auto findRes2 = m_nodeBuffer_.find(lk.id);
        if (findRes2 != m_nodeBuffer_.end())
        {
          m_mapActiveNode.emplace(lk.id, ActNodeInfo{findRes2->second, lk.linkVal});
        }
        else
        {
          auto empRes = m_nodeBuffer_.emplace(lk.id, std::make_unique<NodeInfo>(lk.id));
          m_mapActiveNode.emplace(lk.id, ActNodeInfo{empRes.first->second, lk.linkVal});
        }
      }
    };
    node_type firstNode = {m_idAlloca.allocate(), std::make_unique<NodeInfo>()};

    m_nodeBuffer_.emplace(firstNode);

    for (const auto &item: m_lstIOS_)
      for (const link_t &lk: (*item->inputActInfo())) loadLink(lk);
    for (const link_t &lk: m_actedStatic_) loadLink(lk);
    for (const link_t &lk: m_actedDynamic_) loadLink(lk);
    firstNode.second->m_lsConstLink   = std::move(m_actedConst_);
    firstNode.second->m_lsStaticLink  = std::move(m_actedStatic_);
    firstNode.second->m_lsDynamicLink = std::move(m_actedDynamic_);

    for (const auto &[id, info]: m_mapActiveNode)
    {
      info.info->m_lsDynamicLink.emplace_back(firstNode.first, NUM_DEF_MEMORY);
      info.act(m_actedDynamic_, m_actedStatic_, m_actedConst_,
               io::IOInterface::interference.mentalConcentration * m_focusStand_,
               io::IOInterface::interference.emotionActive);
    }

    return m_mapActiveNode.size();
  }

  void stableThink()
  {
    for (const auto &[id, info]: m_nodeBuffer_)
    {
      info->stableLink(0.5);
      info->writeNodeInfo(id);
    }
    m_nodeBuffer_.clear();
    for (const auto &item: m_lstIOS_) item->initConstId(m_idAlloca, m_nodeBuffer_);
  }

private:
};
}// namespace think

#endif//THINKSYSTEM_IOMANAGE_H
