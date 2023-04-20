//
// Created by HanHaocheng on 2023/04/19.
//

#ifndef THINKSYSTEM_SYSTEM_MANAGE_STATIC_H
#define THINKSYSTEM_SYSTEM_MANAGE_STATIC_H
#include "../system_manage.h"

namespace think {
///静态节点管理系统
class StaticManageSystem final : public NodeManageSystem {
public:
  ~StaticManageSystem() final {
    save_node();
    m_node_data_.clear();
  }

  NodeMapPair create(NodeType type) final {
    auto id = m_id_alloc_.allocate();
    return {m_node_data_.try_emplace(id, std::make_unique<LinkListArray>(NodeType::STATIC)).first->first,
            m_node_data_[id]};
  }

  bool remove(NodeID id) final {
    if (m_node_data_.erase(id) != 0) {
      m_id_alloc_.release(id);
      return true;
    }
    std::string path = id.id_path();
    if (fs::exists(path + "node.dat")) {
      fs::remove(path);
      m_id_alloc_.release(id);
      return true;
    } else {
      return false;
    }
  }

  void saveDynamic(DynamicManageSystem &in, double saveWeight) {
    in.saveDynamicLinks(saveWeight);
    this->saveDynamicLinks(saveWeight);
    this->m_node_data_.insert(in.m_node_data_.begin(), in.m_node_data_.end());
  }

  std::shared_ptr<LinkListArray> getLinkListArray(NodeID id) final {
    auto res = m_node_data_.find(id);
    if (res != m_node_data_.end()) {
      return res->second;
    }
    auto type = read_node(id);
    if (type->m_node_type_ == NodeType::STATIC) {
      return m_node_data_[id] = type;
    } else if (type->m_node_type_ == NodeType::NULL_TYPE) {
      return nullptr;
    } else {
      throw std::logic_error("流程错误，输入id是非静态节点");
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
};//! class StaticManageSystem;

}// namespace think

#endif//THINKSYSTEM_SYSTEM_MANAGE_STATIC_H
