//
// Created by HanHaocheng on 2023/04/19.
//

#ifndef THINKSYSTEM_SYSTEM_MANAGE_DYNAMIC_H
#define THINKSYSTEM_SYSTEM_MANAGE_DYNAMIC_H
#include "../system_manage.h"

namespace think {
class DynamicManageSystem final : public NodeManageSystem {
  friend class StaticManageSystem;

public:
  ~DynamicManageSystem() final {
    m_node_data_.clear();
  }
  NodeMapPair create(NodeType type) final {
    auto id  = m_id_alloc_.allocate();
    auto res = m_node_data_.try_emplace(id, std::make_unique<LinkListArray>(NodeType::DYNAMIC));
    return *res.first;
  }
  bool remove(NodeID id) final {
    if (m_node_data_.erase(id) != 0) {
      m_id_alloc_.release(id);
      return true;
    }
    return false;
  }

  std::shared_ptr<LinkListArray> getLinkListArray(NodeID id) final {
    auto res = m_node_data_.find(id);
    if (res != m_node_data_.end()) {
      return res->second;
    } else {
      return nullptr;
    }
  }

  bool setNode(NodeID id, const std::shared_ptr<LinkListArray> &links) final {
    auto res = m_node_data_.find(id);
    if (res == m_node_data_.end()) {
      return false;
    }
    res->second = links;
    return true;
  }
};//! class DynamicManageSystem

}// namespace think
#endif//THINKSYSTEM_SYSTEM_MANAGE_DYNAMIC_H
