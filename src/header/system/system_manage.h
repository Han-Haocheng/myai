//
// Created by HanHaocheng on 2023/03/31.
//

#ifndef DATA_MANAGE_H_
#define DATA_MANAGE_H_

#include "../type/type_node.h"
#include <memory>
#include <stdexcept>

#include <map>

#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

namespace think {


//============================================================================================================

//节点管理
//申请节点，查找节点，删除节点，修改节点
class NodeManageSystem {
protected:
  std::unordered_map<NodeID, std::shared_ptr<LinkListArray>> m_node_data_;

public:
  virtual ~NodeManageSystem()                                          = 0;
  virtual bool create(NodeID id)                                = 0;
  virtual bool remove(NodeID id)                                = 0;
  virtual std::shared_ptr<LinkListArray> getLinkListArray(NodeID)      = 0;
  virtual bool setNode(NodeID, const std::shared_ptr<LinkListArray> &) = 0;

protected:
  static std::pair<NodeType, std::shared_ptr<LinkListArray>> read_node(const NodeID &id) {
    std::string path = id.id_path() + "node.dat";
    if (!fs::exists(path)) {
      return {NodeType::NULL_TYPE, nullptr};
    }
    std::ifstream file{path, std::ios ::binary | std::ios ::in};
    if (file.is_open()) {
      return {NodeType::NULL_TYPE, nullptr};
    }
    std::shared_ptr<LinkListArray> res = std::make_unique<LinkListArray>();
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
    return {tmpInfo.nType, res};
  }

  static bool write_node(NodeType type, const NodeID &id, const std::shared_ptr<LinkListArray> &links) {
    std::string path = id.id_path();
    if (!fs::exists(path)) {
      fs::create_directories(path);
    }
    std::ofstream file{path + "node.dat", std::ios ::binary | std::ios ::out};
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
            type,
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
    return true;
  }
};//! virtual class NodeManageSystem


class DynamicManageSystem : public NodeManageSystem {
  friend class StaticManageSystem;

public:
  ~DynamicManageSystem() override {
    m_node_data_.clear();
  }

  bool remove(NodeID id) override {
    return m_node_data_.erase(id) > 0;
  }

  std::shared_ptr<LinkListArray> getLinkListArray(NodeID id) override {
    auto res = m_node_data_.find(id);
    if (res != m_node_data_.end()) {
      return res->second;
    } else {
      return nullptr;
    }
  }

  bool setNode(NodeID id, const std::shared_ptr<LinkListArray> &links) override {
    auto res = m_node_data_.find(id);
    if (res == m_node_data_.end()) {
      return false;
    }
    res->second = links;
    return true;
  }

  void saveDynamic(double saveWeight) {
    (void) std::for_each(m_node_data_.begin(),
                         m_node_data_.end(),
                         [&](const std::pair<const NodeID, std::shared_ptr<LinkListArray>> &item) -> void {
                           item.second->saveDynamicData(saveWeight);
                         });
  }
};//! class DynamicManageSystem

///静态节点管理系统
class StaticManageSystem : public NodeManageSystem {
public:
  ~StaticManageSystem() override {
    m_node_data_.clear();
  }

  void dynamicToStatic(DynamicManageSystem &in, double saveWeight) {
    in.saveDynamic(saveWeight);
    this->m_node_data_.insert(in.m_node_data_.begin(), in.m_node_data_.end());
  }

  std::shared_ptr<LinkListArray> getLinkListArray(NodeID id) override {
    auto res = m_node_data_.find(id);
    if (res != m_node_data_.end()) {
      return res->second;
    }
    std::shared_ptr<LinkListArray> tmp;
    auto type = read_node(id);
    if (type.first == NodeType::STATIC) {
      return m_node_data_[id] = type.second;
    } else if (type.first == NodeType::NULL_TYPE) {
      return nullptr;
    } else {
      throw std::logic_error("流程错误，输入id是非静态节点");
    }
  }

  bool setNode(NodeID id, const std::shared_ptr<LinkListArray> &links) override {
    auto res = m_node_data_.find(id);
    if (res == m_node_data_.end()) {
      return false;
    }
    res->second = links;
    return true;
  }

  bool remove(NodeID id) override {
    if (m_node_data_.erase(id)==0){
      return remove_file(id);
    }
    return true;
  }
private:
  static bool remove_file(NodeID id){
    std::string path = id.id_path();
    if (fs::exists(path+"node.dat")){
      fs::remove(path);
      return true;
    } else{
      return false;
    }
  }
};
//! class DynamicManageSystem;

///常态节点管理系统
class ConstManageSystem : public NodeManageSystem {
  std::unordered_map<NodeType, std::vector<LinkListArray>> m_type_id_;

public:
  ConstManageSystem() {
  }

  ~ConstManageSystem() {
  }

  std::shared_ptr<LinkListArray> getLinkListArray(NodeID id) override {
    auto res = m_node_data_.find(id);
    if (res != m_node_data_.end()) {
      return res->second;
    } else {
      return nullptr;
    }
  }

  bool setNode(NodeID id, const std::shared_ptr<LinkListArray> &links) override {
    auto res = m_node_data_.find(id);
    if (res == m_node_data_.end()) {
      return false;
    }
    res->second = links;
    return true;
  }


private:
  void read_const_id() {
  }
  void save_const_id() {
  }
};//! class ConstManageSystem;


}// namespace think

#endif//DATA_MANAGE_H_
