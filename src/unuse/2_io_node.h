//
// Created by HanHaocheng on 2023/04/27.
//

#ifndef THINKSYSTEM_2_IO_NODE_H
#define THINKSYSTEM_2_IO_NODE_H

#include "../think_core/CoreInclude.h"
#include "../think_core/_NodeIDAllocator.h"
#include "1_node_info.h"

namespace think
{

class CNodeIOManage
{
protected:
  NodeIDAllocator m_cIDAlloc_;
  std::unordered_map<node_id, SP_NODE_INFO> m_cMapNodeBuffer_;

public:
  CNodeIOManage() : m_cIDAlloc_(NUM_NODE_ID_MAX) {}

  size_t getBufferSize() const { return m_cMapNodeBuffer_.size(); }


  void saveAllNode(WEIGHT dySaveWeight)
  {
    for (const auto &[id, spLkGp]: m_cMapNodeBuffer_)
    { spLkGp->stableLink(id, dySaveWeight);
    }
  }

  std::pair<const node_id, SP_NODE_INFO> *emplace()
  {
    return &*m_cMapNodeBuffer_.emplace(m_cIDAlloc_.allocate(), std::make_unique<NodeInfo>(NodeInfo{ENodeType::DYNAMIC_TYPE})).first;
  }

  std::pair<const node_id, SP_NODE_INFO> *emplace(const SP_NODE_INFO &spLG)
  {
    return &*m_cMapNodeBuffer_.emplace(m_cIDAlloc_.allocate(), spLG).first;
  }

  std::pair<const node_id, SP_NODE_INFO> *emplace(node_id id, const SP_NODE_INFO &spLG)
  {
    return &*m_cMapNodeBuffer_.emplace(id, spLG).first;
  }

  bool emplace(size_t num, std::vector<std::pair<node_id, SP_NODE_INFO>> &out)
  {
    out.resize(num);
    for (auto &item: out)
    {
      item = *m_cMapNodeBuffer_.emplace(m_cIDAlloc_.allocate(), std::make_unique<NodeInfo>(NodeInfo{ENodeType::DYNAMIC_TYPE})).first;
    }
    return true;
  }

  bool erase(node_id id)
  {
    m_cIDAlloc_.release(id);
    return _remove(id);
  }

  bool erase(const std::vector<node_id> &ids)
  {
    m_cIDAlloc_.release(ids);
    for (const auto &id: ids)
    {
      _remove(id);
    }
    return true;
  }

  std::pair<const node_id, SP_NODE_INFO> *find(node_id id)
  {
    auto fRes = m_cMapNodeBuffer_.find(id);
    if (fRes != m_cMapNodeBuffer_.end())
    {
      ++fRes->second->m_iActNum;
      return &*fRes;
    }
    SP_NODE_INFO rRes;
    return rRes->readNodeInfo(id) ? this->emplace(id, rRes) : nullptr;
  }

private:
  bool _remove(node_id id)
  {
    auto fRes = m_cMapNodeBuffer_.find(id);
    if (fRes == m_cMapNodeBuffer_.end())
    {
      fs::remove_all(GetNodePath(id));
    }
    else
    {
      if (((int) fRes->second->m_type_ & ((int) ENodeType::DYNAMIC_TYPE | (int) ENodeType::STATIC_TYPE)))
      {
        m_cMapNodeBuffer_.erase(fRes);
      }
      else
      {
        return false;
      }
    }
    return true;
  }
};
}// namespace think
#endif//THINKSYSTEM_2_IO_NODE_H
