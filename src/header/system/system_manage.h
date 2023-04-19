//
// Created by HanHaocheng on 2023/03/31.
//

#ifndef DATA_MANAGE_H_
#define DATA_MANAGE_H_

#include "../type/type_node.h"
#include "system_allocator.h"

#include <memory>
#include <stdexcept>

#include <map>

#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;

namespace think {

constexpr const static char *STRING_NODE_FILE = "node.dat";

//============================================================================================================
using NodeMap     = std::unordered_map<NodeID, std::shared_ptr<LinkListArray>>;
using NodeMapPair = std::pair<const NodeID, std::shared_ptr<LinkListArray>>;
//节点管理
//申请节点，查找节点，删除节点，修改节点
class NodeManageSystem {
protected:
  NodeMap m_node_data_;
  NodeIDAllocator &m_id_alloc_;

public:
  explicit NodeManageSystem(NodeIDAllocator &idAlloc) : m_id_alloc_(idAlloc) {}
  std::unique_ptr<NodeMap> getNodeMap() const {
    return std::make_unique<NodeMap>(m_node_data_);
  }

  virtual ~NodeManageSystem()                                          = 0;
  virtual NodeMapPair create(NodeType)                                 = 0;
  virtual bool remove(NodeID id)                                       = 0;
  virtual std::shared_ptr<LinkListArray> getLinkListArray(NodeID)      = 0;
  virtual bool setNode(NodeID, const std::shared_ptr<LinkListArray> &) = 0;

  void saveDynamicLinks(double saveWeight) {
    (void) std::for_each(m_node_data_.begin(),
                         m_node_data_.end(),
                         [&](const std::pair<const NodeID, std::shared_ptr<LinkListArray>> &item) -> void {
                           item.second->saveDynamicData(saveWeight);
                         });
  }

protected:
  static std::shared_ptr<LinkListArray> read_node(const NodeID &id) {
    std::string path = id.id_path();
    if (!fs::exists(path + STRING_NODE_FILE)) {
      return nullptr;
    }
    std::ifstream file{path + STRING_NODE_FILE, std::ios ::binary | std::ios ::in};
    if (file.is_open()) {
      return nullptr;
    }
    std::unique_ptr<LinkListArray> res = std::make_unique<LinkListArray>();
    struct FileInfo {
      NodeType nType;
      size_t const_count;
      size_t const_value;
      size_t static_count;
      size_t static_value;
    } tmpInfo{};
    file.read(reinterpret_cast<char *>(&tmpInfo), sizeof(NodeType));
    res->m_const_link_.link_val  = tmpInfo.const_value;
    res->m_static_link_.link_val = tmpInfo.static_value;
    res->m_const_link_.link.resize(tmpInfo.const_count);
    res->m_static_link_.link.resize(tmpInfo.static_count);
    file.read(reinterpret_cast<char *>(res->m_const_link_.link.data()),
              static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.const_count));
    file.read(reinterpret_cast<char *>(res->m_static_link_.link.data()),
              static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.static_count));
    file.close();
    return res;
  }

  static bool write_node(const NodeID &id, const std::shared_ptr<LinkListArray> &links) {
    std::string path = id.id_path();
    if (!fs::exists(path)) {
      fs::create_directories(path);
    }
    std::ofstream file{path + STRING_NODE_FILE, std::ios ::binary | std::ios ::out};
    if (file.is_open()) {
      return false;
    }

    struct FileInfo {
      NodeType nType;
      size_t const_count;
      size_t const_value;
      size_t static_count;
      size_t static_value;
    } tmpInfo{
            links->m_node_type_,
            links->m_const_link_.link.size(),
            links->m_const_link_.link_val,
            links->m_static_link_.link.size(),
            links->m_static_link_.link_val,
    };

    file.write(reinterpret_cast<const char *>(&tmpInfo), sizeof(NodeType));
    file.write(reinterpret_cast<const char *>(links->m_const_link_.link.data()),
               static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.const_count));
    file.write(reinterpret_cast<const char *>(links->m_static_link_.link.data()),
               static_cast<std::streamsize>(sizeof(LinkInfo) * tmpInfo.static_count));
    file.close();
    return true;
  }

  void save_node() {
    if (m_node_data_.empty()) {
      return;
    }
    for (const auto &item: m_node_data_) {
      write_node(item.first, item.second);
    }
  }
};//! virtual class NodeManageSystem

}// namespace think

#endif//DATA_MANAGE_H_
